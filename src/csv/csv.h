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
    uint32_t timestampMs = 0; // Time since boot, supplied by acquisition.
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
    Full,
    OutputFailed
};

// Mount without formatting. Create the header if absent/empty, otherwise
// validate the file and recover its count. Preserve incompatible/partial files.
// A fresh device needs a LittleFS image or explicit formatting before begin().
Result begin();

// One calibrated spectrum per line. Label may contain commas/quotes, but no
// control characters; at most MAX_LABEL_BYTES UTF-8 bytes. Reject NaN/Inf.
// Write floats with 9 significant digits, decimal point and comma delimiter.
Result append(const Record& record, const char* label);

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
