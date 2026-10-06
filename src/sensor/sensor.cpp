#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"
#include "AS7265X.h"
#include "nvs_manager/nvs_manager.h"

static AS7265X sensor;


uint16_t maxADCvalue;

void setMaxADCvalue(uint16_t value) {
    maxADCvalue = value;
}

uint16_t getMaxADCvalue() {
    return maxADCvalue;
}

bool sensorInit(int sda, int scl) {
    if (!Wire.begin(sda, scl, 400000)) {
        return false;
    }
    return sensor.begin(Wire);
}

void startMeasurements() {
    sensor.setIntegrationCycles(configActual.integracionCiclos);
    sensor.setGain(configActual.ganancia);
    // Runtime() polls for completion so the UI can run during integration.
    sensor.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT);
}

bool isDataReady() {
    return sensor.dataAvailable();
}

void readRawChannels(uint16_t* values) {
    values[0] = getRawChannel1();
    values[1] = getRawChannel2();
    values[2] = getRawChannel3();
    values[3] = getRawChannel4();
    values[4] = getRawChannel5();
    values[5] = getRawChannel6();
    values[6] = getRawChannel7();
    values[7] = getRawChannel8();
    values[8] = getRawChannel9();
    values[9] = getRawChannel10();
    values[10] = getRawChannel11();
    values[11] = getRawChannel12();
    values[12] = getRawChannel13();
    values[13] = getRawChannel14();
    values[14] = getRawChannel15();
    values[15] = getRawChannel16();
    values[16] = getRawChannel17();
    values[17] = getRawChannel18();
}

void readCalibratedChannels(float* values) {
    values[0] = getCalibratedChannel1();
    values[1] = getCalibratedChannel2();
    values[2] = getCalibratedChannel3();
    values[3] = getCalibratedChannel4();
    values[4] = getCalibratedChannel5();
    values[5] = getCalibratedChannel6();
    values[6] = getCalibratedChannel7();
    values[7] = getCalibratedChannel8();
    values[8] = getCalibratedChannel9();
    values[9] = getCalibratedChannel10();
    values[10] = getCalibratedChannel11();
    values[11] = getCalibratedChannel12();
    values[12] = getCalibratedChannel13();
    values[13] = getCalibratedChannel14();
    values[14] = getCalibratedChannel15();
    values[15] = getCalibratedChannel16();
    values[16] = getCalibratedChannel17();
    values[17] = getCalibratedChannel18();
}

void showRawData(bool print) {
  sensor.takeMeasurements(); //This is a hard wait while all 18 channels are measured
  if (print) { 
    Serial.print(sensor.getCalibratedA()); //410nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedB()); //435nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedC()); //460nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedD()); //485nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedE()); //510nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedF()); //535nm
    Serial.print(",");

    Serial.print(sensor.getCalibratedG()); //560nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedH()); //585nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedR()); //610nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedI()); //645nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedS()); //680nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedJ()); //705nm
    Serial.print(",");

    Serial.print(sensor.getCalibratedT()); //730nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedU()); //760nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedV()); //810nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedW()); //860nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedK()); //900nm
    Serial.print(",");
    Serial.print(sensor.getCalibratedL()); //940nm
    Serial.print(",");

    Serial.println(); 
  } 
}

void enableIndicator(bool state) {
  if (state) {
    sensor.enableIndicator();
  } else {
    sensor.disableIndicator();
  }
}

uint16_t getRawChannel1(){
    return sensor.getA();//410nm
}

uint16_t getRawChannel2(){
    return sensor.getB();//435nm
}

uint16_t getRawChannel3(){
    return sensor.getC();//460nm
}

uint16_t getRawChannel4(){
    return sensor.getD();//485nm
}

uint16_t getRawChannel5(){
    return sensor.getE();//510nm
}

uint16_t getRawChannel6(){
    return sensor.getF();//535nm
}

uint16_t getRawChannel7(){
    return sensor.getG();//560nm
}

uint16_t getRawChannel8(){
    return sensor.getH();//585nm
}

uint16_t getRawChannel9(){
    return sensor.getR();//610nm
}

uint16_t getRawChannel10(){
    return sensor.getI();//645nm
}

uint16_t getRawChannel11(){
    return sensor.getS();//680nm
}

uint16_t getRawChannel12(){
    return sensor.getJ();//705nm
}

uint16_t getRawChannel13(){
    return sensor.getT();//730nm
}

uint16_t getRawChannel14(){
    return sensor.getU();//760nm
}

uint16_t getRawChannel15(){
    return sensor.getV();//810nm
}

uint16_t getRawChannel16(){
    return sensor.getW();//860nm
}

uint16_t getRawChannel17(){
    return sensor.getK();//900nm
}

uint16_t getRawChannel18(){
    return sensor.getL();//940nm
}

float getCalibratedChannel1(){
    return sensor.getCalibratedA();//410nm
}

float getCalibratedChannel2(){
    return sensor.getCalibratedB();//435nm
}

float getCalibratedChannel3(){
    return sensor.getCalibratedC();//460nm
}

float getCalibratedChannel4(){
    return sensor.getCalibratedD();//485nm
}

float getCalibratedChannel5(){
    return sensor.getCalibratedE();//510nm
}

float getCalibratedChannel6(){
    return sensor.getCalibratedF();//535nm
}

float getCalibratedChannel7(){
    return sensor.getCalibratedG();//560nm
}

float getCalibratedChannel8(){
    return sensor.getCalibratedH();//585nm
}

float getCalibratedChannel9(){
    return sensor.getCalibratedR();//610nm
}

float getCalibratedChannel10(){
    return sensor.getCalibratedI();//645nm
}

float getCalibratedChannel11(){
    return sensor.getCalibratedS();//680nm
}

float getCalibratedChannel12(){
    return sensor.getCalibratedJ();//705nm
}

float getCalibratedChannel13(){
    return sensor.getCalibratedT();//730nm
}

float getCalibratedChannel14(){
    return sensor.getCalibratedU();//760nm
}

float getCalibratedChannel15(){
    return sensor.getCalibratedV();//810nm
}

float getCalibratedChannel16(){
    return sensor.getCalibratedW();//860nm
}

float getCalibratedChannel17(){
    return sensor.getCalibratedK();//900nm
}

float getCalibratedChannel18(){
    return sensor.getCalibratedL();//940nm
}

byte mapFloatToByte(float x, float in_max) {
  // 1. Calcula la proporción en decimales
  float resultadoDecimal = (x / in_max) * 100.0;
  
  // 2. Redondea al entero más cercano
  int resultadoRedondeado = round(resultadoDecimal);
  
  // 3. Limita el resultado estrictamente entre 0 y 100 y lo devuelve como byte
  //   return (byte)constrain(resultadoRedondeado, 0, 100);
  return (byte)resultadoRedondeado;
}

void setWhiteLEDCurrent(int current) {
  if (current == 1) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_WHITE);
  else if (current == 2) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_WHITE);
  else if (current == 3) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_WHITE);
  else if (current == 4) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_100MA, AS7265x_LED_WHITE);
  sensor.enableBulb(AS7265x_LED_WHITE);
}

void resetWhiteLEDCurrent() {
  sensor.disableBulb(AS7265x_LED_WHITE);
}

void setIRLEDCurrent(int current) {
  if (current == 1) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_IR);
  else if (current == 2) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_IR);
  else if (current == 3) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_IR);
  sensor.enableBulb(AS7265x_LED_IR);
}

void resetIRLEDCurrent() {
  sensor.disableBulb(AS7265x_LED_IR);
}

void setUVLEDCurrent(int current) {
  if (current == 1) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_UV);
  else if (current == 2) sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_UV);
  sensor.enableBulb(AS7265x_LED_UV);
}

void resetUVLEDCurrent() {
  sensor.disableBulb(AS7265x_LED_UV);
}
