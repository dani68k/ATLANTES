#include "csv.h"

#include <LittleFS.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace csv {
namespace {

constexpr const char* TEMP_PATH = "/data.csv.tmp";
constexpr char HEADER[] =
    "sample_id,label,t_ms,cal410,cal435,cal460,cal485,cal510,cal535,"
    "cal560,cal585,cal610,cal645,cal680,cal705,cal730,cal760,"
    "cal810,cal860,cal900,cal940";
constexpr size_t LINE_CAPACITY = 640;

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

bool parseUnsigned(const char*& cursor) {
    if (*cursor < '0' || *cursor > '9') return false;
    char* end = nullptr;
    errno = 0;
    const unsigned long long value = strtoull(cursor, &end, 10);
    if (errno == ERANGE || value > UINT32_MAX || *end != ',') return false;
    cursor = end + 1;
    return true;
}

bool validRow(const char* cursor) {
    if (!parseUnsigned(cursor) || *cursor++ != '"') return false;
    size_t labelBytes = 0;
    for (;;) {
        const unsigned char value = static_cast<unsigned char>(*cursor++);
        if (value < 32 || value == 127) return false;
        if (value == '"') {
            if (*cursor != '"') break;
            ++cursor; // Escaped quote counts as one label byte.
        }
        if (++labelBytes > MAX_LABEL_BYTES) return false;
    }
    if (*cursor++ != ',' || !parseUnsigned(cursor)) return false;
    for (uint8_t i = 0; i < CHANNEL_COUNT; ++i) {
        char* end = nullptr;
        const float value = strtof(cursor, &end);
        if (end == cursor || !isfinite(value)) return false;
        if (i + 1 == CHANNEL_COUNT) return *end == '\0';
        if (*end != ',') return false;
        cursor = end + 1;
    }
    return false;
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
    for (float value : record.calibrated) {
        if (!isfinite(value)) return Result::InvalidRecord;
    }

    char line[LINE_CAPACITY];
    int written = snprintf(line, sizeof(line), "%lu,\"",
                           static_cast<unsigned long>(record.sampleId));
    if (written < 0 || static_cast<size_t>(written) >= sizeof(line)) {
        return Result::InvalidRecord;
    }
    size_t length = static_cast<size_t>(written);
    // Bounds are guaranteed by MAX_LABEL_BYTES and LINE_CAPACITY.
    for (size_t i = 0; i < labelLength; ++i) {
        if (label[i] == '"') line[length++] = '"';
        line[length++] = label[i];
    }
    written = snprintf(line + length, sizeof(line) - length, "\",%lu",
                       static_cast<unsigned long>(record.timestampMs));
    if (written < 0 || static_cast<size_t>(written) >= sizeof(line) - length) {
        return Result::InvalidRecord;
    }
    length += written;
    for (float value : record.calibrated) {
        written = snprintf(line + length, sizeof(line) - length, ",%.9g",
                           static_cast<double>(value));
        if (written < 0 || static_cast<size_t>(written) >= sizeof(line) - length) {
            return Result::InvalidRecord;
        }
        length += written;
    }
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
        case Result::InvalidRecord: return "Invalid label or calibrated values";
        case Result::Full: return "CSV full (100 records)";
        case Result::OutputFailed: return "CSV export failed";
    }
    return "Unknown CSV error";
}

} // namespace csv
