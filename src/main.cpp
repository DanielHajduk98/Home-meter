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

#include "DHT.h"
// #include "RTC.h"
#include "BMP280.h"
#include "PIR.h"
#include "GY30.h"

#define MEASURE_INTERVAL 1000
unsigned long millisCurrent;
unsigned long millisLastPrint = 0;

PIR pir(D5);
DHT dht(D3, DHT11);
GY30 gy30;
BMP280 bmp280;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  dht.begin();
  gy30.begin();
  bmp280.begin();

  // RTCBegin();
}

void loop() {
  millisCurrent = millis();
  
  if((millisCurrent - millisLastPrint) >= MEASURE_INTERVAL) {
    const float temp = bmp280.getTemp();
    const float RH = dht.readHumidity();

    Serial.println("Movement: " + (String)pir.movement);
    Serial.println("Temperature: " + (String)temp + "°C");
    Serial.println("Air pressure: " + (String)bmp280.getPressure() + "hPa");
    Serial.println("Relative humidity: " + (String)RH + "%");
    Serial.println("Heat index: " + (String)dht.computeHeatIndex(temp, RH));
    Serial.println("Lux: " + (String)gy30.getLux() + "lx");   
    Serial.println();

    millisLastPrint = millisCurrent; 

    pir.movement = 0;
  } else if ((millisCurrent - pir.millisLast) >= 200) {
    pir.getMovement();
    pir.millisLast= millisCurrent;
  }
}