#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cstdarg>
#include <cstdio>
#include <string>

class Print {
public:
    virtual ~Print() {}
    virtual size_t write(const uint8_t* data, size_t size) = 0;
};

class MockSerial : public Print {
public:
    std::string input;
    std::string output;
    int available() const { return static_cast<int>(input.size()); }
    int read() {
        if (input.empty()) return -1;
        const unsigned char value = input[0];
        input.erase(0, 1);
        return value;
    }
    size_t write(const uint8_t* data, size_t size) override {
        output.append(reinterpret_cast<const char*>(data), size);
        return size;
    }
    void println(const char* text) { output += text; output += '\n'; }
    void printf(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        output += buffer;
    }
};

extern MockSerial Serial;
