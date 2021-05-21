#include <WiFiManager.h> 
#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <BMP280.h>
#include <PIR.h>
#include <GY30.h>
#include <API.h>
#include <EEPROM.h>

#include <Display.h>

#define MEASURE_INTERVAL 10000
#define POST_INTERVAL 1000 * 60 * 15
#define PIR_INTERVAL 3000
#define BTN_DEBOUNCE_TIME 200

WiFiManager wifiManager;
PIR pir(D5);
DHT dht(D3, DHT11);
GY30 gy30;
BMP280 bmp280;
API api("https://labkom-api.herokuapp.com");
Display display = Display(128, 64, &Wire);

// Time
unsigned long millisCurrent;
unsigned long millisLastMeasurement = 0;
unsigned long millisLastPost = 0;

// Measurement
float temp = 0;
float RH = 0;
float HI = 0;
unsigned int pressure = 0;
unsigned int movement = 0;
unsigned int lightLevel = 0;

// Connection
int connectionCode = -1;
unsigned int btnDebounce = 0;
short retry = 0;

void collectMeasurements() {
  temp = bmp280.readTemperature();
  pressure = bmp280.readPressure() / 100;
  RH = dht.readHumidity();
  HI = dht.computeHeatIndex(temp, RH);
  lightLevel = gy30.readLightLevel();

  millisLastMeasurement = millisCurrent; 
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode callback");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("No saved WiFi found.");
  display.println("Connect to network:");
  display.println(myWiFiManager->getConfigPortalSSID());
  display.println("Then go to this URL:");
  display.println(WiFi.softAPIP());
  display.display();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// Setup + Loop
// +++++++++++++++++++++++++++++++++++++++++++++++++++
void setup() {
  Serial.begin(115200);

  pinMode(D6, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.println("Loading...");
  display.display();

  display.println("Starting Wifi...");
  display.display();
  
  // reset settings - wipe credentials for testing
  // wifiManager.resetSettings();
  WiFi.setAutoConnect(true);
  WiFi.persistent(true);

  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect("Labkom")) {
    Serial.println("Failed to connect and hit timeout");

    display.println("Connection failed!");
    display.display();
  } else {
    display.println("Connected to:");
    display.println(WiFi.SSID());
    display.display();
  }
  
  pir.init();

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
  
  while (connectionCode >= 300 || connectionCode < 0) {
    connectionCode = api.setup();

    if (connectionCode >= 300 || connectionCode < 0) {
      display.clear();
      display.println("Connection failed");
      display.println("Http code: " + (String)connectionCode);
      display.println("Retrying connection");
      display.display();
      delay(5000); 
    }
  }

  connectionCode = -1;
  display.clear();
  display.println("Connection success!");
  display.display();
  delay(1000);

  collectMeasurements();
}

void loop() {  
  while (WiFi.status() != WL_CONNECTED) {
    if (retry == 3) retry = 0;

    display.clear();
    display.println("WiFi signal lost!");
    display.print("Reconnecting");
    for (short i = 0; i <= retry; i++){
      display.print(".");
    }
    display.println("");
    display.display();
    delay(1000);
    retry++;
  }

  millisCurrent = millis();

  //Send measurement
  if((millisCurrent - millisLastPost) >= POST_INTERVAL) {
    retry = 0;
    
    while (connectionCode >= 300 || connectionCode < 0) {
      collectMeasurements();

      connectionCode = api.sendMeasurements(
        temp,
        RH,
        pressure,
        lightLevel,
        movement,
        HI
      );

      if(connectionCode >= 300 || connectionCode < 0) {
        if (retry == 3) retry = 0;

        display.clear();
        display.println("Connection failed!");
        display.println("Http code: " + (String)connectionCode);
        display.print("Trying to resend");
        for (short i = 0; i <= retry; i++){
          display.print(".");
        }
        display.println("");
        display.display();

        retry++;
        delay(5000);
      }
    }

    connectionCode = -1;
    movement = 0;
    millisLastPost = millis();
  } 

  //Get movement every 3s
  if ((millisCurrent - pir.millisLast) >= PIR_INTERVAL) {
    movement += pir.read();
    pir.millisLast = millisCurrent;
  }

  // Toggle screen on btn press
  if (digitalRead(D6)){

    // Debounce prevention
    if ((millisCurrent - btnDebounce) >= BTN_DEBOUNCE_TIME) {
      if(display.toggleScreen()) {
        collectMeasurements();
        display.displayMeasurements(POST_INTERVAL, millisCurrent, millisLastPost, temp, RH, HI, pressure, lightLevel, movement);
      }
    }

    btnDebounce = millisCurrent;
  }
  
  //Collect measurement when screen is turned on
  if(display.turnedOn) {
    if ((millisCurrent - millisLastMeasurement) >= MEASURE_INTERVAL){
      collectMeasurements();
    }

    display.displayMeasurements(POST_INTERVAL, millisCurrent, millisLastPost, temp, RH, HI, pressure, lightLevel, movement);
  }  
}
