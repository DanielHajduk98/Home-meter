#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void DHTBegin() {
    dht.begin();
}

float getHI(float t, float RH) {
  return dht.computeHeatIndex(t, RH, false);
}

float getRH() {
  const float RH = dht.readHumidity();

  if (isnan(RH)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }

  return RH;
}

float getAltTemp(bool inF = false) {
  const float temp = dht.readTemperature(inF);
  
  if (isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }

  return temp;
}