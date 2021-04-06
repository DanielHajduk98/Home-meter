#include <WiFiManager.h> 
#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <BMP280.h>
#include <PIR.h>
#include <GY30.h>
#include <API.h>

#define MEASURE_INTERVAL 10000 // 15 min

WiFiManager wifiManager;
PIR pir(D5);
DHT dht(D3, DHT11);
GY30 gy30;
BMP280 bmp280;
API api("http://192.168.1.27:81/api");

unsigned long millisCurrent;
unsigned long millisLastMeasurement = 0;

float temp = 0;
float RH = 0;
float HI = 0;
unsigned int pressure = 0;
unsigned int movement = 0;
unsigned int lightLevel = 0;

void collectMeasurements() {
  temp = bmp280.readTemperature();
  pressure = bmp280.readPressure() / 100;
  RH = dht.readHumidity();
  HI = dht.computeHeatIndex(temp, RH);
  lightLevel = gy30.readLightLevel();

  millisLastMeasurement = millisCurrent; 
}

void printMeasurements() {
  Serial.println("Movement: " + (String)movement);
  Serial.println("Temperature: " + (String)temp + "°C");
  Serial.println("Air pressure: " + (String)pressure + "hPa");
  Serial.println("Relative humidity: " + (String)RH + "%");
  Serial.println("Heat index: " + (String)HI);
  Serial.println("Lux: " + (String)lightLevel + "lx");   
  Serial.println();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// Setup + Loop
// +++++++++++++++++++++++++++++++++++++++++++++++++++

void setup() {
  wifiManager.autoConnect("Home-meter");

  // reset settings - wipe credentials for testing
  // wifiManager.resetSettings();

  Serial.begin(115200);
  Wire.begin();

  dht.begin(); 
  gy30.begin();
  bmp280.begin();
  api.setup();
}

void loop() {  
  millisCurrent = millis();
  
  if((millisCurrent - millisLastMeasurement) >= MEASURE_INTERVAL) {
    collectMeasurements();
    
    printMeasurements();

    api.sendMeasurements(
      temp,
      RH,
      pressure,
      lightLevel,
      movement,
      HI
    );

    movement = 0;
  } 
  else if ((millisCurrent - pir.millisLast) >= 3000) {
    movement += pir.read();
    pir.millisLast = millisCurrent;
  }
}
