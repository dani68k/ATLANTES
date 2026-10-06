#ifndef SENSOR_H
#define SENSOR_H

void setMaxADCvalue(uint16_t value);
uint16_t getMaxADCvalue();
bool sensorInit(int sda, int scl);
void showRawData(bool print);
void enableIndicator(bool state);
uint16_t getRawChannel1();
uint16_t getRawChannel2();
uint16_t getRawChannel3();
uint16_t getRawChannel4();
uint16_t getRawChannel5();
uint16_t getRawChannel6();
uint16_t getRawChannel7();
uint16_t getRawChannel8();
uint16_t getRawChannel9();
uint16_t getRawChannel10();
uint16_t getRawChannel11();
uint16_t getRawChannel12();
uint16_t getRawChannel13();
uint16_t getRawChannel14();
uint16_t getRawChannel15();
uint16_t getRawChannel16();
uint16_t getRawChannel17();
uint16_t getRawChannel18();
float getCalibratedChannel1();
float getCalibratedChannel2();
float getCalibratedChannel3();
float getCalibratedChannel4();
float getCalibratedChannel5();
float getCalibratedChannel6();
float getCalibratedChannel7();
float getCalibratedChannel8();
float getCalibratedChannel9();
float getCalibratedChannel10();
float getCalibratedChannel11();
float getCalibratedChannel12();
float getCalibratedChannel13();
float getCalibratedChannel14();
float getCalibratedChannel15();
float getCalibratedChannel16();
float getCalibratedChannel17();
float getCalibratedChannel18();
byte mapFloatToByte(float x, float in_max);
void setWhiteLEDCurrent(int current);
void resetWhiteLEDCurrent();
bool isDataReady();
void startMeasurements();
// Each destination must hold 18 channels, ordered from 410 to 940 nm.
void readRawChannels(uint16_t* values);
void readCalibratedChannels(float* values);
void setIRLEDCurrent(int current);
void resetIRLEDCurrent();
void setUVLEDCurrent(int current);
void resetUVLEDCurrent();
#endif
