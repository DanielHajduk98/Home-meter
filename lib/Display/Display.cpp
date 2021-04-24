#include "Display.h"

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


Display::Display(byte w, byte h, TwoWire *wire)
    : Adafruit_SSD1306(w, h, wire)
{
    turnedOn = true;
}

void Display::begin(uint8_t vcs, uint8_t addr){
  if(!Adafruit_SSD1306::begin(vcs, addr)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  Adafruit_SSD1306::clearDisplay();
  Adafruit_SSD1306::setTextSize(1);
  Adafruit_SSD1306::setTextColor(SSD1306_WHITE);
  Adafruit_SSD1306::setCursor(0, 0);
  Adafruit_SSD1306::cp437(true);
  Adafruit_SSD1306::println("Loading...");
  Adafruit_SSD1306::display();
}

void Display::drawHeatIndexEmoji(float HI) {
  if (26 >= HI && HI <= 32) {
        Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), emoji_neutral, 8, 8, WHITE);
      } else if (32 < HI && HI <= 41) {
        Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), emoji_sad, 8, 8, WHITE);
      } else if (41 < HI && HI <= 54) {
        Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), emoji_scared, 8, 8, WHITE);
      } else if (HI > 54) {
        Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), emoji_danger, 8, 8, WHITE);
      }
  Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), emoji_smile, 8, 8, WHITE); 
}

void Display::displayMeasurements(unsigned int POST_INTERVAL, unsigned long millisCurrent, unsigned long millisLastPost, float temp, float RH, float HI, unsigned int pressure, unsigned int lightLevel, unsigned int movement) {
  int progress = ((millisCurrent - millisLastPost) * 128) / (POST_INTERVAL); 

  Adafruit_SSD1306::setCursor(0,0);
  Adafruit_SSD1306::clearDisplay();

  
  Adafruit_SSD1306::drawRect(0, 0, 128, 8, WHITE);
  Adafruit_SSD1306::fillRect(2, 2, progress - 4 , 4, WHITE);
    
  Adafruit_SSD1306::println("");
  Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), arrow_up, 8, 8, WHITE);
  Adafruit_SSD1306::setCursor(Adafruit_SSD1306::getCursorX() + 8, Adafruit_SSD1306::getCursorY());
  Adafruit_SSD1306::print("  Until next save ");
  Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), arrow_up, 8, 8, WHITE); 
  Adafruit_SSD1306::setCursor(Adafruit_SSD1306::getCursorX() + 8, Adafruit_SSD1306::getCursorY());
  Adafruit_SSD1306::println("");
  Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), arrow_down, 8, 8, WHITE);
  Adafruit_SSD1306::setCursor(Adafruit_SSD1306::getCursorX() + 8, Adafruit_SSD1306::getCursorY());
  Adafruit_SSD1306::print("     Current      "); 
  Adafruit_SSD1306::drawBitmap(Adafruit_SSD1306::getCursorX(), Adafruit_SSD1306::getCursorY(), arrow_down, 8, 8, WHITE);
  Adafruit_SSD1306::println("");

  Adafruit_SSD1306::print("Temperature: " + (String)temp); Adafruit_SSD1306::write(248); Adafruit_SSD1306::println("C");
  Adafruit_SSD1306::println("Humidity: " + (String)RH + "%");
  Adafruit_SSD1306::println("Mov: " + (String)movement + " Lum: " + (String)lightLevel + " Lux");
  Adafruit_SSD1306::println("Atm Pressure: " + (String)pressure + "hPa");
  Adafruit_SSD1306::print("Heat Index: "); Display::drawHeatIndexEmoji(HI); Adafruit_SSD1306::println("");

  Adafruit_SSD1306::display();
}

void Display::clear() {
  Adafruit_SSD1306::clearDisplay();
  Adafruit_SSD1306::setCursor(0,0); 
}

void Display::toggleScreen() {
  turnedOn = !turnedOn;

  if (!turnedOn){
    Display::clear();
    Adafruit_SSD1306::display();
  }
  
}