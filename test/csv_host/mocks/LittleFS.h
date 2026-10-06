#pragma once
#include "Arduino.h"
#include <algorithm>
#include <cstring>
#include <map>
#include <memory>
#include <string>

#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"

struct MockNode { std::string data; };

class File {
    std::shared_ptr<MockNode> node;
    size_t cursor = 0;
    size_t* writeLimit = nullptr;
    bool* readFailure = nullptr;
public:
    File() {}
    File(std::shared_ptr<MockNode> entry, size_t start, size_t* limit, bool* fail)
        : node(entry), cursor(start), writeLimit(limit), readFailure(fail) {}
    explicit operator bool() const { return !!node; }
    bool isDirectory() const { return false; }
    size_t position() const { return cursor; }
    size_t size() const { return node ? node->data.size() : 0; }
    int read() {
        if (!node || *readFailure || cursor >= size()) return -1;
        return static_cast<unsigned char>(node->data[cursor++]);
    }
    size_t read(uint8_t* data, size_t capacity) {
        if (!node || *readFailure) return 0;
        const size_t count = std::min(capacity, size() - cursor);
        memcpy(data, node->data.data() + cursor, count);
        cursor += count;
        return count;
    }
    size_t write(const uint8_t* data, size_t count) {
        if (!node) return 0;
        count = std::min(count, *writeLimit);
        node->data.replace(cursor, count, reinterpret_cast<const char*>(data), count);
        cursor += count;
        return count;
    }
    size_t write(uint8_t value) { return write(&value, 1); }
    void flush() {}
    void close() { node.reset(); }
};

class MockLittleFS {
public:
    std::map<std::string, std::shared_ptr<MockNode>> files;
    bool mountOk = true;
    bool openOk = true;
    bool renameOk = true;
    bool failRead = false;
    bool formatRequested = false;
    size_t writeLimit = static_cast<size_t>(-1);
    bool begin(bool formatOnFail) { formatRequested = formatOnFail; return mountOk; }
    bool exists(const char* path) { return files.count(path) != 0; }
    File open(const char* path, const char* mode) {
        if (!openOk) return File();
        if (*mode == 'r' && !exists(path)) return File();
        if (!exists(path)) files[path] = std::make_shared<MockNode>();
        if (*mode == 'w') files[path]->data.clear();
        const size_t start = *mode == 'a' ? files[path]->data.size() : 0;
        return File(files[path], start, &writeLimit, &failRead);
    }
    bool rename(const char* from, const char* to) {
        if (!renameOk || !exists(from)) return false;
        files[to] = files[from];
        files.erase(from);
        return true;
    }
};

extern MockLittleFS LittleFS;
