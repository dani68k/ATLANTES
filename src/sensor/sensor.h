#ifndef SENSOR_H
#define SENSOR_H

bool sensorInit(int sda, int scl);
void showRawData(bool print);
void enableIndicator(bool state);
float getDataChannel1();
float getDataChannel2();
float getDataChannel3();
float getDataChannel4();
float getDataChannel5();
float getDataChannel6();
float getDataChannel7();
float getDataChannel8();
float getDataChannel9();
float getDataChannel10();
float getDataChannel11();
float getDataChannel12();
float getDataChannel13();
float getDataChannel14();
float getDataChannel15();
float getDataChannel16();
float getDataChannel17();
float getDataChannel18();
byte mapFloatToByte(float x, float in_max);
void setWhiteLEDCurrent(int current);
void resetWhiteLEDCurrent();
bool isDataReady();
void startMeasurements();
#endif