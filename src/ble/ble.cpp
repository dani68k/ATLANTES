#include "ble.h"

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <LittleFS.h>
#include "csv/csv.h"

namespace ble {
namespace {

constexpr char DEVICE_NAME[] = "AS7265x CSV";
constexpr char SERVICE_UUID[] = "9c5e1000-7e5a-4f2d-9a91-30e52e7b1b01";
constexpr char CONTROL_UUID[] = "9c5e1001-7e5a-4f2d-9a91-30e52e7b1b01";
constexpr char DATA_UUID[] = "9c5e1002-7e5a-4f2d-9a91-30e52e7b1b01";
constexpr char STATUS_UUID[] = "9c5e1003-7e5a-4f2d-9a91-30e52e7b1b01";
constexpr char CSV_REQUEST[] = "GET_CSV";
constexpr size_t BLE_CHUNK_SIZE = 20;
constexpr uint32_t CHUNK_INTERVAL_MS = 10;
constexpr UBaseType_t COMMAND_QUEUE_LENGTH = 4;

enum class Command : uint8_t {
    GetCsv,
    Invalid
};

BLEServer* server = nullptr;
BLECharacteristic* dataCharacteristic = nullptr;
BLECharacteristic* statusCharacteristic = nullptr;
QueueHandle_t commandQueue = nullptr;
File transferFile;
bool transferActive = false;
bool clientConnected = false;
bool initialized = false;
volatile bool commandQueueFull = false;
uint32_t transferSize = 0;
uint32_t bytesSent = 0;
uint32_t lastChunkAt = 0;

class ServerCallbacks final : public BLEServerCallbacks {
    void onConnect(BLEServer*) override {
        clientConnected = true;
    }

    void onDisconnect(BLEServer*) override {
        clientConnected = false;
        BLEDevice::startAdvertising();
    }
};

class ControlCallbacks final : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic) override {
        const String value = characteristic->getValue();
        const Command command =
            value == CSV_REQUEST ? Command::GetCsv : Command::Invalid;
        if (xQueueSend(commandQueue, &command, 0) != pdTRUE) {
            commandQueueFull = true;
        }
    }
};

void sendStatus(const char* status) {
    statusCharacteristic->setValue(status);
    if (clientConnected) statusCharacteristic->notify();
}

void startTransfer() {
    if (transferActive) {
        sendStatus("BUSY");
        return;
    }

    transferFile = LittleFS.open(csv::FILE_PATH, FILE_READ);
    if (!transferFile || transferFile.isDirectory()) {
        if (transferFile) transferFile.close();
        sendStatus("ERROR:OPEN");
        return;
    }

    transferSize = transferFile.size();
    bytesSent = 0;
    lastChunkAt = 0;
    transferActive = true;
    sendStatus("BUSY");
}

void processCommand(Command command) {
    if (command == Command::GetCsv) {
        startTransfer();
    } else {
        sendStatus("ERROR:COMMAND");
    }
}

void sendNextChunk() {
    if (!transferActive) return;
    if (!clientConnected) {
        transferFile.close();
        transferActive = false;
        return;
    }

    const uint32_t now = millis();
    if (now - lastChunkAt < CHUNK_INTERVAL_MS) return;
    lastChunkAt = now;

    uint8_t chunk[BLE_CHUNK_SIZE];
    const size_t count = transferFile.read(chunk, sizeof(chunk));
    if (count == 0) {
        transferFile.close();
        transferActive = false;
        sendStatus("ERROR:READ");
        return;
    }

    dataCharacteristic->setValue(chunk, count);
    dataCharacteristic->notify();
    bytesSent += static_cast<uint32_t>(count);

    if (bytesSent == transferSize) {
        transferFile.close();
        transferActive = false;
        char status[24];
        snprintf(status, sizeof(status), "DONE:%lu",
                 static_cast<unsigned long>(bytesSent));
        sendStatus(status);
    }
}

} // namespace

bool begin() {
    if (initialized) return true;

    if (!commandQueue) {
        commandQueue = xQueueCreate(COMMAND_QUEUE_LENGTH, sizeof(Command));
    }
    if (!commandQueue) return false;

    BLEDevice::init(DEVICE_NAME);
    server = BLEDevice::createServer();
    if (!server) return false;
    server->setCallbacks(new ServerCallbacks());

    BLEService* service = server->createService(SERVICE_UUID);
    if (!service) return false;

    BLECharacteristic* controlCharacteristic = service->createCharacteristic(
        CONTROL_UUID, BLECharacteristic::PROPERTY_WRITE);
    dataCharacteristic = service->createCharacteristic(
        DATA_UUID, BLECharacteristic::PROPERTY_NOTIFY);
    statusCharacteristic = service->createCharacteristic(
        STATUS_UUID, BLECharacteristic::PROPERTY_READ |
                         BLECharacteristic::PROPERTY_NOTIFY);
    if (!controlCharacteristic || !dataCharacteristic || !statusCharacteristic) {
        return false;
    }

    controlCharacteristic->setCallbacks(new ControlCallbacks());
    dataCharacteristic->addDescriptor(new BLE2902());
    statusCharacteristic->addDescriptor(new BLE2902());
    statusCharacteristic->setValue("READY");
    service->start();

    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    BLEDevice::startAdvertising();
    initialized = true;
    return true;
}

void loop() {
    if (!commandQueue || !statusCharacteristic) return;

    if (commandQueueFull) {
        commandQueueFull = false;
        sendStatus("ERROR:BUSY");
    }

    Command command;
    while (xQueueReceive(commandQueue, &command, 0) == pdTRUE) {
        processCommand(command);
    }

    sendNextChunk();
}

} // namespace ble