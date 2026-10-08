#include "csv.h"

#include <LittleFS.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

namespace csv {
namespace {

constexpr const char* TEMP_PATH = "/data.csv.tmp";
constexpr char HEADER[] =
    "Sample_id,Label,White LED,UV LED,IR LED,Gain,Integration time,Measurement time,Temperature,"
    "raw410,raw435,raw460,raw485,raw510,raw535,raw560,raw585,raw610,raw645,raw680,raw705,raw730,raw760,raw810,raw860,raw900,raw940,"
    "cal410,cal435,cal460,cal485,cal510,cal535,cal560,cal585,cal610,cal645,cal680,cal705,cal730,cal760,cal810,cal860,cal900,cal940";
constexpr size_t LINE_CAPACITY = 1024;

bool mounted = false;
bool ready = false;
uint16_t recordCount = 0;

Result readLine(File& file, char* line, size_t capacity) {
    size_t length = 0;
    while (file.position() < file.size()) {
        const int value = file.read();
        if (value < 0) return Result::ReadFailed;
        if (value == '\n') {
            if (length && line[length - 1] == '\r') --length;
            line[length] = '\0';
            return Result::Ok;
        }
        if (value == 0 || length + 1 >= capacity) return Result::InvalidData;
        line[length++] = static_cast<char>(value);
    }
    // Every committed row must end in a newline, including the header.
    return Result::InvalidData;
}

bool parseUnsigned(const char*& cursor, char delimiter, uint32_t maximum) {
    if (*cursor < '0' || *cursor > '9') return false;
    char* end = nullptr;
    errno = 0;
    const unsigned long long value = strtoull(cursor, &end, 10);
    if (errno == ERANGE || value > maximum || *end != delimiter) return false;
    cursor = delimiter == '\0' ? end : end + 1;
    return true;
}

bool parseFloat(const char*& cursor, char delimiter) {
    if (*cursor == '\0') return false;
    char* end = nullptr;
    const float value = strtof(cursor, &end);
    if (end == cursor || !isfinite(value) || *end != delimiter) return false;
    cursor = delimiter == '\0' ? end : end + 1;
    return true;
}

bool parseQuotedField(const char*& cursor, size_t maximumBytes) {
    if (*cursor++ != '"') return false;
    size_t labelBytes = 0;
    for (;;) {
        const unsigned char value = static_cast<unsigned char>(*cursor);
        if (value == '\0') return false;
        ++cursor;
        if (value < 32 || value == 127) return false;
        if (value == '"') {
            if (*cursor != '"') {
                if (*cursor++ != ',') return false;
                return true;
            }
            ++cursor;
        }
        if (++labelBytes > maximumBytes) return false;
    }
}

bool parseTextField(const char*& cursor, size_t maximumBytes) {
    size_t length = 0;
    while (*cursor != ',' && *cursor != '\0') {
        const unsigned char value = static_cast<unsigned char>(*cursor++);
        if (value < 32 || value == 127 || value == '"' || ++length > maximumBytes) {
            return false;
        }
    }
    if (*cursor++ != ',') return false;
    return true;
}

bool validRow(const char* cursor) {
    if (!parseUnsigned(cursor, ',', UINT32_MAX) ||
        !parseQuotedField(cursor, MAX_LABEL_BYTES) ||
        !parseTextField(cursor, sizeof(Record{}.whiteLed) - 1) ||
        !parseTextField(cursor, sizeof(Record{}.uvLed) - 1) ||
        !parseTextField(cursor, sizeof(Record{}.irLed) - 1) ||
        !parseTextField(cursor, sizeof(Record{}.gain) - 1) ||
        !parseFloat(cursor, ',') ||
        !parseUnsigned(cursor, ',', UINT32_MAX) ||
        !parseFloat(cursor, ',')) {
        return false;
    }
    for (uint8_t i = 0; i < CHANNEL_COUNT; ++i) {
        if (!parseUnsigned(cursor, ',', UINT16_MAX)) return false;
    }
    for (uint8_t i = 0; i < CHANNEL_COUNT; ++i) {
        if (!parseFloat(cursor, i + 1 == CHANNEL_COUNT ? '\0' : ',')) return false;
    }
    return true;
}

bool safeRecordText(const char* value, size_t capacity) {
    size_t length = 0;
    while (length < capacity && value[length] != '\0') {
        const unsigned char character = static_cast<unsigned char>(value[length]);
        if (character < 32 || character == 127 || character == ',' || character == '"') {
            return false;
        }
        ++length;
    }
    return length < capacity;
}

bool appendFormatted(char* line, size_t capacity, size_t& length,
                     const char* format, ...) {
    if (length >= capacity) return false;
    va_list arguments;
    va_start(arguments, format);
    const int written = vsnprintf(line + length, capacity - length, format, arguments);
    va_end(arguments);
    if (written < 0 || static_cast<size_t>(written) >= capacity - length) return false;
    length += static_cast<size_t>(written);
    return true;
}

Result scanFile() {
    File file = LittleFS.open(FILE_PATH, FILE_READ);
    if (!file) return Result::OpenFailed;
    if (file.isDirectory()) return Result::InvalidData;
    char line[LINE_CAPACITY];
    Result result = readLine(file, line, sizeof(line));
    if (result != Result::Ok) return result;
    if (strcmp(line, HEADER) != 0) return Result::InvalidData;

    uint16_t count = 0;
    while (file.position() < file.size()) {
        result = readLine(file, line, sizeof(line));
        if (result != Result::Ok) return result;
        if (!validRow(line) || ++count > MAX_RECORDS) return Result::InvalidData;
    }
    recordCount = count;
    return Result::Ok;
}

Result replaceWithHeader() {
    // Stage the header before replacing the existing log.
    File file = LittleFS.open(TEMP_PATH, FILE_WRITE);
    if (!file) return Result::OpenFailed;
    const size_t length = sizeof(HEADER) - 1;
    const bool written =
        file.write(reinterpret_cast<const uint8_t*>(HEADER), length) == length &&
        file.write(static_cast<uint8_t>('\n')) == 1;
    file.flush();
    file.close();
    if (!written) return Result::WriteFailed;
    if (!LittleFS.rename(TEMP_PATH, FILE_PATH)) return Result::RenameFailed;
    return Result::Ok;
}

} // namespace

Result begin() {
    ready = false;
    recordCount = 0;
    mounted = LittleFS.begin(false);
    if (!mounted) return Result::MountFailed;

    bool needsHeader = !LittleFS.exists(FILE_PATH);
    if (!needsHeader) {
        File file = LittleFS.open(FILE_PATH, FILE_READ);
        if (!file) return Result::OpenFailed;
        if (file.isDirectory()) return Result::InvalidData;
        needsHeader = file.size() == 0;
    }
    if (needsHeader) {
        const Result result = replaceWithHeader();
        if (result != Result::Ok) return result;
    }
    const Result result = scanFile();
    ready = result == Result::Ok;
    return result;
}

Result append(const Record& record, const char* label) {
    if (!ready) return Result::NotInitialized;
    if (recordCount >= MAX_RECORDS) return Result::Full;
    if (!label) return Result::InvalidRecord;

    size_t labelLength = 0;
    while (label[labelLength]) {
        const unsigned char value = static_cast<unsigned char>(label[labelLength]);
        if (labelLength >= MAX_LABEL_BYTES || value < 32 || value == 127) {
            return Result::InvalidRecord;
        }
        ++labelLength;
    }
    if (!safeRecordText(record.whiteLed, sizeof(record.whiteLed)) ||
        !safeRecordText(record.uvLed, sizeof(record.uvLed)) ||
        !safeRecordText(record.irLed, sizeof(record.irLed)) ||
        !safeRecordText(record.gain, sizeof(record.gain)) ||
        !isfinite(record.integrationTimeMs) || !isfinite(record.temperature)) {
        return Result::InvalidRecord;
    }
    for (float value : record.calibrated) {
        if (!isfinite(value)) return Result::InvalidRecord;
    }

    char line[LINE_CAPACITY];
    size_t length = 0;
    if (!appendFormatted(line, sizeof(line), length, "%lu,\"",
                         static_cast<unsigned long>(record.sampleId))) {
        return Result::InvalidRecord;
    }
    for (size_t i = 0; i < labelLength; ++i) {
        if (label[i] == '"') line[length++] = '"';
        line[length++] = label[i];
    }
    if (!appendFormatted(line, sizeof(line), length, "\",%s,%s,%s,%s,%.2f,%lu,%.2f",
                         record.whiteLed, record.uvLed, record.irLed, record.gain,
                         record.integrationTimeMs,
                         record.measureTime,
                         record.temperature)) {
        return Result::InvalidRecord;
    }
    for (uint16_t value : record.raw) {
        if (!appendFormatted(line, sizeof(line), length, ",%u",
                             static_cast<unsigned>(value))) {
            return Result::InvalidRecord;
        }
    }
    for (float value : record.calibrated) {
        if (!appendFormatted(line, sizeof(line), length, ",%.4f",
                             static_cast<double>(value))) {
            return Result::InvalidRecord;
        }
    }
    if (length + 1 >= sizeof(line)) return Result::InvalidRecord;
    line[length++] = '\n';

    if (!LittleFS.exists(FILE_PATH)) {
        ready = false;
        return Result::OpenFailed;
    }
    File file = LittleFS.open(FILE_PATH, FILE_APPEND);
    if (!file) return Result::OpenFailed;
    const size_t bytesWritten = file.write(reinterpret_cast<const uint8_t*>(line), length);
    file.flush();
    file.close();
    if (bytesWritten != length) {
        // Do not append beyond a potentially partial row.
        ready = false;
        return Result::WriteFailed;
    }
    ++recordCount;
    return Result::Ok;
}

Result getRecordCount(uint16_t& count) {
    if (!ready) return Result::NotInitialized;
    count = recordCount;
    return Result::Ok;
}

Result exportTo(Print& output) {
    if (!mounted) return Result::NotInitialized;
    // Permit export of an invalid log so the caller can recover its contents.
    File file = LittleFS.open(FILE_PATH, FILE_READ);
    if (!file) return Result::OpenFailed;
    if (file.isDirectory()) return Result::InvalidData;
    uint8_t buffer[128];
    while (file.position() < file.size()) {
        const size_t count = file.read(buffer, sizeof(buffer));
        if (count == 0 || count > sizeof(buffer)) return Result::ReadFailed;
        size_t sent = 0;
        while (sent < count) {
            const size_t amount = output.write(buffer + sent, count - sent);
            if (amount == 0 || amount > count - sent) return Result::OutputFailed;
            sent += amount;
        }
    }
    return Result::Ok;
}

Result clear() {
    if (!mounted) return Result::NotInitialized;
    const Result result = replaceWithHeader();
    if (result != Result::Ok) return result;
    ready = false;
    const Result scanned = scanFile();
    ready = scanned == Result::Ok;
    return scanned;
}

const char* resultMessage(Result result) {
    switch (result) {
        case Result::Ok: return "OK";
        case Result::NotInitialized: return "CSV not initialized";
        case Result::MountFailed: return "LittleFS mount failed";
        case Result::OpenFailed: return "Cannot open CSV file";
        case Result::ReadFailed: return "CSV read failed";
        case Result::WriteFailed: return "CSV write failed";
        case Result::RenameFailed: return "Cannot replace CSV file";
        case Result::InvalidData: return "Invalid or incomplete CSV file";
        case Result::InvalidRecord: return "Invalid CSV record";
        case Result::Full: return "CSV full (100 records)";
        case Result::OutputFailed: return "CSV export failed";
    }
    return "Unknown CSV error";
}

} // namespace csv
