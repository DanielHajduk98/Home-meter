#include <Arduino.h>
#include "DHT11.h"
#include "RTC.h"
#include "BMP280.h"

#define MEASURE_INTERVAL 10000

void setup() {
  Serial.begin(115200);

BMP280Begin();
  DHTBegin();
  RTCBegin();
}

void loop() {
  delay(MEASURE_INTERVAL);

  readBMP280();
  readDHT(); 
  getTime();
}