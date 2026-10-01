#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"
#include "AS7265X.h"
#include "nvs_manager/nvs_manager.h"

static AS7265X sensor;

bool sensorInit(int sda, int scl) {
    if (!Wire.begin(sda, scl, 400000)) {
        return false;
    }
    return sensor.begin(Wire);
}

void startMeasurements() {
    sensor.setIntegrationCycles(configActual.integracionCiclos);
    sensor.setGain(configActual.ganancia);
    sensor.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT);
}

bool isDataReady() {
    return sensor.dataAvailable();
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

float getDataChannel1(){
    return sensor.getCalibratedA();
}

float getDataChannel2(){
    return sensor.getCalibratedB();
}

float getDataChannel3(){
    return sensor.getCalibratedC();
}

float getDataChannel4(){
    return sensor.getCalibratedD();
}

float getDataChannel5(){
    return sensor.getCalibratedE();
}

float getDataChannel6(){
    return sensor.getCalibratedF();
}

float getDataChannel7(){
    return sensor.getCalibratedG();
}

float getDataChannel8(){
    return sensor.getCalibratedH();
}

float getDataChannel9(){
    return sensor.getCalibratedR();
}

float getDataChannel10(){
    return sensor.getCalibratedI();
}

float getDataChannel11(){
    return sensor.getCalibratedS();
}

float getDataChannel12(){
    return sensor.getCalibratedJ();
}

float getDataChannel13(){
    return sensor.getCalibratedT();
}

float getDataChannel14(){
    return sensor.getCalibratedU();
}

float getDataChannel15(){
    return sensor.getCalibratedV();
}

float getDataChannel16(){
    return sensor.getCalibratedW();
}

float getDataChannel17(){
    return sensor.getCalibratedK();
}

float getDataChannel18(){
    return sensor.getCalibratedL();
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