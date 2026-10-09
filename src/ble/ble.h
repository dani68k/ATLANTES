#ifndef SPECTROMETER_BLE_H
#define SPECTROMETER_BLE_H

namespace ble {

// Start advertising the BLE CSV service.
bool begin(const char* firmwareVersion);

// Process queued commands and send at most one CSV data chunk.
void loop();

} // namespace ble

#endif