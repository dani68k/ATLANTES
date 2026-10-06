# CSV host checks

These tests compile the production `src/csv/csv.cpp` with in-memory Arduino and
LittleFS substitutes. They do not access the ESP32 or emulate flash persistence
and power-loss behavior. Run from the project root with a host C++ compiler:

```text
g++ -std=c++11 -Itest/csv_host/mocks -Isrc src/csv/csv.cpp test/csv_host/main.cpp -o .pio/csv_host_tests.exe
.pio/csv_host_tests.exe
```

The tests cover the 100-record limit (plus header), restoring the counter,
label escaping, finite float extremes, rejection of invalid/incomplete data,
short writes, export errors, and explicit clear without formatting.

The terminal-command checks compile the production serial handler as well:

```text
g++ -std=c++11 -Itest/csv_host/mocks -Isrc src/csv/csv.cpp src/app/serial_commands.cpp test/csv_host/serial_commands.cpp -o .pio/csv_serial_tests.exe
.pio/csv_serial_tests.exe
```

They check rejection while LIVE or a pending acquisition is active, exact CSV
export with CR/LF/CRLF, fragmented commands, backspace, unknown commands, and
bounded input handling. They do not simulate the touchscreen or real UART timing.
