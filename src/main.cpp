#include <WiFiManager.h> 
#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <BMP280.h>
#include <PIR.h>
#include <GY30.h>
#include <API.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#define MEASURE_INTERVAL 10000
#define POST_INTERVAL 1000 * 60 * 15

WiFiManager wifiManager;
PIR pir(D5);
DHT dht(D3, DHT11);
GY30 gy30;
BMP280 bmp280;
API api("http://192.168.1.27:81/api");
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

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

static const unsigned char PROGMEM emoji_smile[] = {
 B00000000,
 B00000000,
 B00100100,
 B00000000,
 B00000000,
 B01000010,
 B00111100,
 B00000000,
};

static const unsigned char PROGMEM emoji_neutral[] = {
 B00000000,
 B00000000,
 B00100100,
 B00000000,
 B00000000,
 B01111110,
 B00000000,
 B00000000,
};

static const unsigned char PROGMEM emoji_sad[] = {
 B00000000,
 B00000000,
 B00100100,
 B00000000,
 B00000000,
 B00111100,
 B01000010,
 B00000000,
};

static const unsigned char PROGMEM emoji_scared[] = {
 B00000000,
 B00000000,
 B00100100,
 B00000000,
 B00000000,
 B00111100,
 B01111110,
 B00000000,
};

static const unsigned char PROGMEM emoji_danger[] = {
 B11011011,
 B11011011,
 B11011011,
 B11011011,
 B11011011,
 B00000000,
 B11011011,
 B11011011,
};

static const unsigned char PROGMEM arrow_up[] = {
 B00000000,
 B00011000,
 B00111100,
 B01111110,
 B00011000,
 B00011000,
 B00011000,
 B00000000,
};

static const unsigned char PROGMEM arrow_down[] = {
 B00000000,
 B00011000,
 B00011000,
 B00011000,
 B01111110,
 B00111100,
 B00011000,
 B00000000,
};

void drawHeatIndexEmoji() {
  if (26 >= HI && HI <= 32) {
        display.drawBitmap(display.getCursorX(), display.getCursorY(), emoji_neutral, 8, 8, WHITE);
      } else if (32 < HI && HI <= 41) {
        display.drawBitmap(display.getCursorX(), display.getCursorY(), emoji_sad, 8, 8, WHITE);
      } else if (41 < HI && HI <= 54) {
        display.drawBitmap(display.getCursorX(), display.getCursorY(), emoji_scared, 8, 8, WHITE);
      } else if (HI > 54) {
        display.drawBitmap(display.getCursorX(), display.getCursorY(), emoji_danger, 8, 8, WHITE);
      }
  display.drawBitmap(display.getCursorX(), display.getCursorY(), emoji_smile, 8, 8, WHITE); 
}


void displayMeasurements() {
  int progress = ((millisCurrent - millisLastPost) * 128) / (POST_INTERVAL); 

  display.setCursor(0,0);
  display.clearDisplay();

  
  display.drawRect(0, 0, 128, 8, WHITE);
  display.fillRect(2, 2, progress - 4 , 4, WHITE);
    
  display.println("");
  display.drawBitmap(display.getCursorX(), display.getCursorY(), arrow_up, 8, 8, WHITE);
  display.setCursor(display.getCursorX() + 8, display.getCursorY());
  display.print("Next save");
  display.drawBitmap(display.getCursorX(), display.getCursorY(), arrow_up, 8, 8, WHITE); 
  display.setCursor(display.getCursorX() + 8, display.getCursorY());
  display.drawBitmap(display.getCursorX(), display.getCursorY(), arrow_down, 8, 8, WHITE);
  display.setCursor(display.getCursorX() + 8, display.getCursorY());
  display.print("Current"); 
  display.drawBitmap(display.getCursorX(), display.getCursorY(), arrow_down, 8, 8, WHITE);
  display.println("");

  display.print("Temperature: " + (String)temp); display.write(248); display.println("C");
  display.println("Humidity: " + (String)RH + "%");
  display.println("Mov: " + (String)movement + " Lum: " + (String)lightLevel + " Lux");
  display.println("Atm Pressure: " + (String)pressure + "hPa");
  display.print("Heat Index: "); drawHeatIndexEmoji(); display.println("");

  display.display();
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
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  display.println("Loading...");
  display.display();

  Serial.begin(115200);

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

  dht.begin() ;
  if(!dht.readTemperature()){
    display.println("DHT init error");
    display.display();
  }

  display.println("Starting GY30...");
  display.display();
  if(!gy30.begin()) {
    display.println("GY30 init error");
    display.display();  
  }

  display.println("Starting BMP280...");
  display.display();
  if(!bmp280.begin()){
    display.println("BMP280 init error");
    display.display();
  }

  display.println("Checking server...");
  display.display();
  api.setup();

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
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Lost WiFi connection!");
    display.display();
  } else {
    displayMeasurements();
  }
}
