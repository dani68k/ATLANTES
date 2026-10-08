#ifndef SPECTROMETER_CSV_H
#define SPECTROMETER_CSV_H

#include <Arduino.h>

// Synchronous API: call from the main task. The module owns /data.csv.
namespace csv {

constexpr uint8_t CHANNEL_COUNT = 18;
constexpr uint16_t MAX_RECORDS = 100; // Header is not counted.
constexpr size_t MAX_LABEL_BYTES = 64;
constexpr const char* FILE_PATH = "/data.csv";

struct Record {
    uint32_t sampleId = 0;
    char whiteLed[5] = {};
    char uvLed[5] = {};
    char irLed[5]  = {};
    char gain[4] = {}; // setup of the measure
    float integrationTimeMs = 0.0f; // setup of the measure
    uint32_t measureTime = 0; // setup of the measure
    float temperature = 0.0f;
    uint16_t raw[CHANNEL_COUNT] = {};
    float calibrated[CHANNEL_COUNT] = {}; // 410..940 nm, in display order.
};

enum class Result : uint8_t {
    Ok,
    NotInitialized,
    MountFailed,
    OpenFailed,
    ReadFailed,
    WriteFailed,
    RenameFailed,
    InvalidData,
    InvalidRecord,
    RecordNotFound,
    StaleRecord,
    Full,
    OutputFailed
};

// Mount without formatting. Create the header if absent/empty, otherwise
// validate the file and recover its count. Preserve incompatible/partial files.
// A fresh device needs a LittleFS image or explicit formatting before begin().
Result begin();

// One spectrum per line. At capacity, discard the oldest row before saving.
// Label may contain commas/quotes, but no control characters; at most
// MAX_LABEL_BYTES UTF-8 bytes. Reject NaN/Inf.
Result append(const Record& record, const char* label);

// Delete a 0-based row in oldest-first order if its sampleId still matches.
Result deleteRecordAt(uint16_t rowIndex, uint32_t expectedSampleId);

// Count is usable only on Ok; it is recovered after reboot by begin().
Result getRecordCount(uint16_t& count);

// Stream the complete file, including its header (e.g. to Serial).
Result exportTo(Print& output);

// Explicitly replace this CSV with its header; do not format LittleFS.
// Also allowed after begin() found invalid CSV data but mounted successfully.
Result clear();

const char* resultMessage(Result result);

} // namespace csv

#endif
