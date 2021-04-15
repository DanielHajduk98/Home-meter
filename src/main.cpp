#include <WiFiManager.h> 
#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <BMP280.h>
#include <PIR.h>
#include <GY30.h>
#include <API.h>

#include <Display.h>

#define MEASURE_INTERVAL 10000
#define POST_INTERVAL 1000 * 60 * 15

WiFiManager wifiManager;
PIR pir(D5);
DHT dht(D3, DHT11);
GY30 gy30;
BMP280 bmp280;
API api("http://192.168.1.27:81/api");
Display display = Display(128, 64, &Wire);

unsigned long millisCurrent;
unsigned long millisLastMeasurement = 0;
unsigned long millisLastPost = 0;

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

void configModeCallback (WiFiManager *myWiFiManager) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("No saved WiFi found.");
  display.println("Connect to network:");
  display.println(myWiFiManager->getConfigPortalSSID());
  display.println("Then go to this URL:");
  display.println(WiFi.softAPIP());
  display.display();
}

void saveConfigFallback () {
  Serial.println("Entered save confiig falback mode");
  display.clearDisplay();
  display.setCursor(0,0);
  display.display();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// Setup + Loop
// +++++++++++++++++++++++++++++++++++++++++++++++++++
void setup() {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.println("Starting Wifi...");
  display.display();
  
  // reset settings - wipe credentials for testing
  // wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigFallback);

  if (!wifiManager.autoConnect("Home-meter")) {
    Serial.println("Failed to connect and hit timeout");

    display.println("Connection failed!");
    display.display();
  } else {
    display.println("Connected to:");
    display.println(WiFi.SSID());
    display.display();
  }
  
  display.println("Starting DHT...");
  display.display();

  dht.begin();

  if(isnan(dht.readTemperature())) {
    display.clear();
    display.println("DHT init error!");
    display.println("Check DHT connection!");
    display.display();

    while (isnan(dht.readTemperature())){
      delay(1000);
    } 

    display.clear();
  }

  display.println("Starting GY30...");
  display.display();
  while(!gy30.begin()) {
    delay(1000);
  }

  display.println("Starting BMP280...");
  display.display();
  while (!bmp280.begin()){
   delay(1000);
  }
   
  display.println("Checking server...");
  display.display();
  
  switch (api.setup()){
  case -1:
    display.clear();
    display.println("Server offline");
    display.display();
    delay(1000);
    break;
  
  default:
    break;
  }

  collectMeasurements();
}

void loop() {  
  millisCurrent = millis();

  if((millisCurrent - millisLastMeasurement) >= MEASURE_INTERVAL) {
    collectMeasurements();
  }
  if((millisCurrent - millisLastPost) >= POST_INTERVAL) {
    collectMeasurements();
  
    api.sendMeasurements(
      temp,
      RH,
      pressure,
      lightLevel,
      movement,
      HI
    );

    movement = 0;
    millisLastPost = millisCurrent;
  } 
  if ((millisCurrent - pir.millisLast) >= 3000) {
    movement += pir.read();
    pir.millisLast = millisCurrent;
  }

  if (WiFi.status() == 6)
  {
    display.clear();
    display.println("Lost WiFi connection!");
    display.display();
  } else {
    display.displayMeasurements(POST_INTERVAL, millisCurrent, millisLastPost, temp, RH, HI, pressure, lightLevel, movement);
  }
}
