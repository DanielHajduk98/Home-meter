#include <Arduino.h>
#include "DHT11.h"
#include "RTC.h"

#define MEASURE_INTERVAL 10000

void setup() {
  Serial.begin(9600);

  DHTBegin();
  RTCBegin();
}

void loop() {
  delay(MEASURE_INTERVAL);

  readDHT(); 
  getTime();
}