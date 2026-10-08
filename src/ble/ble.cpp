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
constexpr char DELETE_PREFIX[] = "D,";
constexpr size_t BLE_CHUNK_SIZE = 20;
constexpr uint32_t NOTIFICATION_INTERVAL_MS = 10;
constexpr UBaseType_t COMMAND_QUEUE_LENGTH = 4;
constexpr UBaseType_t STATUS_QUEUE_LENGTH = 8;
constexpr size_t STATUS_MESSAGE_LENGTH = 24;

enum class CommandType : uint8_t {
    GetCsv,
    DeleteRecord,
    Invalid
};

struct Command {
    CommandType type = CommandType::Invalid;
    uint16_t rowIndex = 0;
    uint32_t sampleId = 0;
};

BLEServer* server = nullptr;
BLECharacteristic* dataCharacteristic = nullptr;
BLECharacteristic* statusCharacteristic = nullptr;
QueueHandle_t commandQueue = nullptr;
QueueHandle_t statusQueue = nullptr;
File transferFile;
bool transferActive = false;
bool clientConnected = false;
bool initialized = false;
volatile bool commandQueueFull = false;
uint32_t transferSize = 0;
uint32_t bytesSent = 0;
uint32_t lastNotificationAt = 0;
bool notificationSent = false;

bool parseUnsigned(const char*& cursor, char delimiter, uint32_t maximum,
                   uint32_t& value) {
    if (*cursor < '0' || *cursor > '9') return false;

    uint32_t parsed = 0;
    do {
        const uint8_t digit = static_cast<uint8_t>(*cursor - '0');
        if (parsed > (maximum - digit) / 10) return false;
        parsed = parsed * 10 + digit;
        ++cursor;
    } while (*cursor >= '0' && *cursor <= '9');

    if (delimiter == '\0') {
        if (*cursor != '\0') return false;
    } else if (*cursor++ != delimiter) {
        return false;
    }
    value = parsed;
    return true;
}

bool parseDeleteCommand(const String& value, Command& command) {
    if (value.length() <= sizeof(DELETE_PREFIX) - 1 ||
        value[0] != DELETE_PREFIX[0] || value[1] != DELETE_PREFIX[1]) {
        return false;
    }

    const char* cursor = value.c_str() + sizeof(DELETE_PREFIX) - 1;
    uint32_t rowIndex = 0;
    uint32_t sampleId = 0;
    if (!parseUnsigned(cursor, ',', csv::MAX_RECORDS - 1, rowIndex) ||
        !parseUnsigned(cursor, '\0', UINT32_MAX, sampleId)) {
        return false;
    }
    command.type = CommandType::DeleteRecord;
    command.rowIndex = static_cast<uint16_t>(rowIndex);
    command.sampleId = sampleId;
    return true;
}

class ServerCallbacks final : public BLEServerCallbacks {
    void onConnect(BLEServer*) override {
        clientConnected = true;
    }

    void onDisconnect(BLEServer*) override {
        clientConnected = false;
        if (statusQueue) xQueueReset(statusQueue);
        BLEDevice::startAdvertising();
    }
};

class ControlCallbacks final : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic) override {
        const String value = characteristic->getValue();
        Command command;
        if (value == CSV_REQUEST) {
            command.type = CommandType::GetCsv;
        } else if (!parseDeleteCommand(value, command)) {
            command.type = CommandType::Invalid;
        }
        if (xQueueSend(commandQueue, &command, 0) != pdTRUE) {
            commandQueueFull = true;
        }
    }
};

void sendStatus(const char* status) {
    if (!clientConnected) return;
    char message[STATUS_MESSAGE_LENGTH];
    const int length = snprintf(message, sizeof(message), "%s", status);
    if (length < 0 || static_cast<size_t>(length) >= sizeof(message)) {
        Serial.printf("[BLE] Status message is too long: %s\n", status);
        return;
    }
    if (xQueueSend(statusQueue, message, 0) != pdTRUE) {
        Serial.printf("[BLE] Status queue full; dropped: %s\n", status);
    }
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
    transferActive = true;
    sendStatus("BUSY");
}

void processCommand(Command command) {
    if (command.type == CommandType::GetCsv) {
        startTransfer();
        return;
    }
    if (command.type == CommandType::DeleteRecord) {
        if (transferActive) {
            sendStatus("ERROR:BUSY");
            return;
        }

        const csv::Result result =
            csv::deleteRecordAt(command.rowIndex, command.sampleId);
        switch (result) {
            case csv::Result::Ok:
                sendStatus("DELETED");
                return;
            case csv::Result::RecordNotFound:
                sendStatus("ERROR:NOT_FOUND");
                return;
            case csv::Result::StaleRecord:
                sendStatus("ERROR:STALE");
                return;
            default:
                sendStatus("ERROR:DELETE");
                return;
        }
    }
    sendStatus("ERROR:COMMAND");
}

void sendNextChunk() {
    if (!transferActive) return;
    if (!clientConnected) {
        transferFile.close();
        transferActive = false;
        return;
    }

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
    lastNotificationAt = millis();
    notificationSent = true;
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

bool sendNextStatus() {
    char status[STATUS_MESSAGE_LENGTH];
    if (xQueuePeek(statusQueue, status, 0) != pdTRUE) return false;

    const uint32_t now = millis();
    if (notificationSent &&
        now - lastNotificationAt < NOTIFICATION_INTERVAL_MS) {
        return false;
    }

    if (xQueueReceive(statusQueue, status, 0) != pdTRUE) return false;
    statusCharacteristic->setValue(status);
    statusCharacteristic->notify();
    lastNotificationAt = millis();
    notificationSent = true;
    return true;
}

} // namespace

bool begin() {
    if (initialized) return true;

    if (!commandQueue) {
        commandQueue = xQueueCreate(COMMAND_QUEUE_LENGTH, sizeof(Command));
    }
    if (!statusQueue) {
        statusQueue = xQueueCreate(STATUS_QUEUE_LENGTH, STATUS_MESSAGE_LENGTH);
    }
    if (!commandQueue || !statusQueue) return false;

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
    if (!commandQueue || !statusQueue || !statusCharacteristic) return;

    if (commandQueueFull) {
        commandQueueFull = false;
        sendStatus("ERROR:BUSY");
    }

    Command command;
    while (xQueueReceive(commandQueue, &command, 0) == pdTRUE) {
        processCommand(command);
    }

    if (sendNextStatus()) return;
    if (uxQueueMessagesWaiting(statusQueue) != 0) return;

    if (notificationSent &&
        millis() - lastNotificationAt < NOTIFICATION_INTERVAL_MS) {
        return;
    }
    sendNextChunk();
}

} // namespace ble