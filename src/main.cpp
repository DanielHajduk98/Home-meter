#include <Arduino.h>
#include "DHT11.h"

#define MEASURE_INTERVAL 10000

void setup() {
  Serial.begin(9600);

  DHTBegin();
}

void loop() {
  delay(MEASURE_INTERVAL);

  readDHT(); 
}