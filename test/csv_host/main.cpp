#include "csv/csv.h"
#include <LittleFS.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>

MockLittleFS LittleFS;

#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #condition); \
    return 1; } } while (0)

class Output : public Print {
public:
    std::string data;
    size_t limit = 7; // Exercise partial writes to the export destination.
    size_t write(const uint8_t* bytes, size_t count) override {
        count = std::min(count, limit);
        data.append(reinterpret_cast<const char*>(bytes), count);
        return count;
    }
};

int main() {
    using csv::Result;
    uint16_t count = 999;
    csv::Record record;
    record.sampleId = UINT32_MAX;
    record.timestampMs = 12345;
    for (unsigned i = 0; i < csv::CHANNEL_COUNT; ++i) record.calibrated[i] = i + 0.125f;

    CHECK(csv::append(record, "before init") == Result::NotInitialized);
    CHECK(csv::getRecordCount(count) == Result::NotInitialized && count == 999);
    CHECK(csv::clear() == Result::NotInitialized);
    LittleFS.mountOk = false;
    CHECK(csv::begin() == Result::MountFailed);
    CHECK(!LittleFS.formatRequested);
    LittleFS.mountOk = true;
    CHECK(csv::begin() == Result::Ok);
    CHECK(csv::getRecordCount(count) == Result::Ok && count == 0);
    const std::string header = LittleFS.files[csv::FILE_PATH]->data;
    CHECK(header.find("cal410,cal435") != std::string::npos);
    CHECK(header.find("cal900,cal940\n") != std::string::npos);

    CHECK(csv::append(record, "sample, \"A\"") == Result::Ok);
    CHECK(LittleFS.files[csv::FILE_PATH]->data.find("4294967295,\"sample, \"\"A\"\"\",12345,0.125,1.125") != std::string::npos);
    CHECK(csv::begin() == Result::Ok); // Recover the count from persistent contents.
    CHECK(csv::getRecordCount(count) == Result::Ok && count == 1);
    CHECK(csv::append(record, "bad\nlabel") == Result::InvalidRecord);
    CHECK(csv::append(record, "bad\rlabel") == Result::InvalidRecord);
    CHECK(csv::append(record, nullptr) == Result::InvalidRecord);
    CHECK(csv::append(record, std::string(65, 'a').c_str()) == Result::InvalidRecord);
    CHECK(csv::append(record, std::string(64, '"').c_str()) == Result::Ok);
    CHECK(csv::begin() == Result::Ok);
    record.calibrated[0] = std::numeric_limits<float>::quiet_NaN();
    CHECK(csv::append(record, "NaN") == Result::InvalidRecord);
    record.calibrated[0] = std::numeric_limits<float>::infinity();
    CHECK(csv::append(record, "Inf") == Result::InvalidRecord);
    record.calibrated[0] = std::numeric_limits<float>::max();
    record.calibrated[1] = std::numeric_limits<float>::denorm_min();
    CHECK(csv::append(record, "extremes") == Result::Ok);
    CHECK(csv::begin() == Result::Ok);
    CHECK(csv::getRecordCount(count) == Result::Ok && count == 3);
    for (uint16_t i = count; i < csv::MAX_RECORDS; ++i) {
        record.sampleId = i;
        CHECK(csv::append(record, "") == Result::Ok);
    }
    const std::string full = LittleFS.files[csv::FILE_PATH]->data;
    CHECK(std::count(full.begin(), full.end(), '\n') == 101);
    CHECK(csv::append(record, "overflow") == Result::Full);
    CHECK(LittleFS.files[csv::FILE_PATH]->data == full);
    CHECK(csv::begin() == Result::Ok);
    CHECK(csv::getRecordCount(count) == Result::Ok && count == 100);
    CHECK(csv::append(record, "still full") == Result::Full);
    Output output;
    CHECK(csv::exportTo(output) == Result::Ok && output.data == full);
    output.limit = 0;
    CHECK(csv::exportTo(output) == Result::OutputFailed);

    LittleFS.renameOk = false;
    CHECK(csv::clear() == Result::RenameFailed);
    CHECK(LittleFS.files[csv::FILE_PATH]->data == full);
    CHECK(csv::getRecordCount(count) == Result::Ok && count == 100);
    LittleFS.renameOk = true;
    CHECK(csv::clear() == Result::Ok);
    CHECK(LittleFS.files[csv::FILE_PATH]->data == header);
    CHECK(csv::getRecordCount(count) == Result::Ok && count == 0);

    LittleFS.openOk = false;
    CHECK(csv::append(record, "open failure") == Result::OpenFailed);
    LittleFS.openOk = true;
    LittleFS.writeLimit = 5;
    CHECK(csv::append(record, "short write") == Result::WriteFailed);
    CHECK(csv::append(record, "blocked") == Result::NotInitialized);
    LittleFS.writeLimit = static_cast<size_t>(-1);
    const std::string partial = LittleFS.files[csv::FILE_PATH]->data;
    CHECK(csv::begin() == Result::InvalidData);
    CHECK(LittleFS.files[csv::FILE_PATH]->data == partial);
    output = Output();
    CHECK(csv::exportTo(output) == Result::Ok && output.data == partial);
    CHECK(csv::clear() == Result::Ok);

    LittleFS.files[csv::FILE_PATH]->data = "foreign header\n";
    CHECK(csv::begin() == Result::InvalidData);
    CHECK(LittleFS.files[csv::FILE_PATH]->data == "foreign header\n");
    CHECK(csv::clear() == Result::Ok);
    CHECK(csv::append(record, "valid") == Result::Ok);
    LittleFS.files[csv::FILE_PATH]->data.pop_back();
    CHECK(csv::begin() == Result::InvalidData); // Missing final newline.
    CHECK(csv::clear() == Result::Ok);
    LittleFS.failRead = true;
    CHECK(csv::begin() == Result::ReadFailed);
    LittleFS.failRead = false;
    CHECK(csv::begin() == Result::Ok);

    LittleFS.files[csv::FILE_PATH]->data.clear(); // Empty data/data.csv image.
    CHECK(csv::begin() == Result::Ok);
    CHECK(LittleFS.files[csv::FILE_PATH]->data == header);
    std::puts("CSV host checks passed: limits, restart, escaping, invalid data, I/O errors and clear.");
    return 0;
}
