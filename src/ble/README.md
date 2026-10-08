# BLE CSV service

This module exposes the ESP32 as a BLE peripheral so a browser using Web
Bluetooth can receive and download `/data.csv`. The browser client is
`webBLE/index.html`; serve it from a secure origin such as `http://localhost`
or HTTPS. BLE does not serve the web page.

## Firmware API

Include `ble/ble.h` and call the functions from the main application task:

| Function | Purpose |
| --- | --- |
| `ble::begin()` | Create the GATT service and start BLE advertising. Returns `false` if initialization fails. |
| `ble::loop()` | Process browser commands and send a pending CSV transfer. Call frequently from `loop()`. |

The CSV module must be initialized separately with `csv::begin()`. BLE can
advertise if CSV initialization fails. A transfer reports `ERROR:OPEN` if the
file cannot be opened; it streams the file even if CSV validation failed so the
browser can recover its contents.

The project uses PlatformIO's `no_ota.csv` partition table. Its 2 MB
application partition accommodates the BLE firmware, but firmware updates must
be performed over USB/serial rather than OTA. This table moves and enlarges
LittleFS compared with the default OTA layout. Export and back up `/data.csv`
before flashing the new partition table; rebuild and upload the LittleFS image
with PlatformIO afterward. `uploadfs` replaces the contents of that partition.

## GATT protocol

The advertised device name is `AS7265x CSV`.

| GATT item | UUID | Properties | Purpose |
| --- | --- | --- | --- |
| CSV service | `9c5e1000-7e5a-4f2d-9a91-30e52e7b1b01` | — | Groups the CSV transfer characteristics |
| Control | `9c5e1001-7e5a-4f2d-9a91-30e52e7b1b01` | Write | Write the UTF-8 command `GET_CSV` to start a transfer |
| CSV data | `9c5e1002-7e5a-4f2d-9a91-30e52e7b1b01` | Notify | Receives consecutive binary fragments of the CSV |
| Status | `9c5e1003-7e5a-4f2d-9a91-30e52e7b1b01` | Read, Notify | Transfer state and final byte count |

Subscribe to notifications on both CSV data and Status before writing the
control command. Data notifications contain up to 20 bytes and have no added
framing; concatenate their byte values in arrival order and decode the result
as UTF-8 after the transfer completes.

Status values:

| Value | Meaning |
| --- | --- |
| `READY` | Service is available |
| `BUSY` | CSV transfer is in progress |
| `DONE:<bytes>` | Transfer completed; `<bytes>` is the expected CSV length |
| `ERROR:OPEN` | `/data.csv` could not be opened |
| `ERROR:READ` | CSV read failed |
| `ERROR:COMMAND` | Unsupported control command |
| `ERROR:BUSY` | Command queue is full |

The client should compare the received byte count with `DONE:<bytes>` before
displaying or downloading the CSV. A disconnect cancels the active transfer;
the peripheral resumes advertising.

## Browser requirements

Web Bluetooth is available only in compatible browsers and secure contexts.
For local development, serve `webBLE/index.html` from `localhost` (for example,
using VS Code Live Server); opening the page through an arbitrary HTTP origin
will not enable Bluetooth access. The ESP32 only provides BLE and does not
host the HTML page.

## Validation

Build the firmware from the project root:

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32dev
```

On hardware, verify discovery, control writes, notification subscription,
full-file byte count and download. Also check disconnects during transfer and
confirm normal sensor/UI operation continues while chunks are being sent.
