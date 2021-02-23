#include <Arduino.h>
#include "DHT11.h"
#include "BMP280.h"

#define MEASURE_INTERVAL 10000

void setup() {
  Serial.begin(9600);

  DHTBegin();
  BMP280Begin();
}

void loop() {
  delay(MEASURE_INTERVAL);

  readDHT(); 
  //readBMP280();
}