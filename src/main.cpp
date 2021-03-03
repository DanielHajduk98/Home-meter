// Measured parameters:
// Temperature (BMP280)
// Alt temperature (DHT-11). Less precise, using BMP instead.
// Humidity (DHT-11)
// Air pressure (BMP280)
// Sound (problem with measuring db)
// Lux intensity (GW-30)
// Time (DS1302). Might resign from using this sensor as it might not be needed
// CO levels. Problem with proper calibration of MQ-7 sensor
// Movement levels (PIR HC501). Constant measurements.
// Heat index (T via BMP280 and RH via DHT-11)

#include <Arduino.h>
#include <Wire.h>

#include "DHT11.h"
// #include "RTC.h"
#include "BMP280.h"
#include "GY30.h"

#define MEASURE_INTERVAL 10000
unsigned long millisCurrent;
unsigned long millisLastPrint = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  GY30Begin();
  BMP280Begin();
  DHTBegin();
  // RTCBegin();
}

void loop() {
  millisCurrent = millis();
  
  if((millisCurrent - millisLastPrint) >= MEASURE_INTERVAL) {
    const float t = getTemp();
    const float RH = getRH();

    Serial.println("Temperature: " + (String)t + "°C");
    Serial.println("Air pressure: " + (String)getPressure() + "hPa");
    Serial.println("Relative humidity: " + (String)RH + "%");
    Serial.println("Heat index: " + (String)getHI(t, RH));
    Serial.println("Lux: " + (String)getLx() + "lx");   
    Serial.println();

    millisLastPrint = millisCurrent; 
  }
}