#include "app/app.h"
#include <LittleFS.h>
#include <cstdio>

MockLittleFS LittleFS;
MockSerial Serial;
bool liveState = false;
bool measureamentInProgress = false;
bool firstMeasureament = false;

#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #condition); \
    return 1; } } while (0)

static void send(const std::string& command) {
    Serial.output.clear();
    Serial.input = command;
    while (Serial.available()) processSerialCommands();
}

int main() {
    send("csv\n");
    CHECK(Serial.output.find("Export failed") != std::string::npos);
    CHECK(csv::begin() == csv::Result::Ok);
    csv::Record record;
    record.sampleId = 7;
    record.timestampMs = 1234;
    record.calibrated[0] = 12.25f;
    CHECK(csv::append(record, "sample, \"A\"") == csv::Result::Ok);
    const std::string saved = LittleFS.files[csv::FILE_PATH]->data;

    liveState = true;
    send("csv\n");
    CHECK(Serial.output.find("Busy") != std::string::npos);
    CHECK(Serial.output.find("sample_id") == std::string::npos);
    liveState = false;
    measureamentInProgress = true;
    send("csv\r\n");
    CHECK(Serial.output.find("Busy") != std::string::npos);
    CHECK(Serial.output.find("sample_id") == std::string::npos);
    measureamentInProgress = false;

    send("csv");
    CHECK(Serial.output.empty()); // Wait for a terminator without blocking.
    send("\n");
    CHECK(Serial.output == "[CSV BEGIN /data.csv]\n" + saved + "[CSV END]\n");
    send("  csv\t\r\n");
    CHECK(Serial.output == "[CSV BEGIN /data.csv]\n" + saved + "[CSV END]\n");
    send("csv\r");
    CHECK(Serial.output.find(saved) != std::string::npos);
    send("csx\bv\n");
    CHECK(Serial.output.find(saved) != std::string::npos);
    send("unknown\n");
    CHECK(Serial.output.find("Unknown command") != std::string::npos);
    CHECK(Serial.output.find(saved) == std::string::npos);
    send(std::string(80, 'x') + "csv\n");
    CHECK(Serial.output.find("Command too long") != std::string::npos);
    CHECK(Serial.output.find(saved) == std::string::npos);
    send("csv\n"); // Recover after the discarded line.
    CHECK(Serial.output.find(saved) != std::string::npos);
    CHECK(LittleFS.files[csv::FILE_PATH]->data == saved);

    Serial.input = std::string(100, ' ');
    processSerialCommands();
    CHECK(Serial.available() == 68); // Input processing is bounded per loop.
    std::puts("Serial CSV checks passed: LIVE/pending guard, framing, partial input and overflow.");
    return 0;
}
