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

/*!
    @brief  Extended Adafruit_SSD1306 class constructor. 
*/
Display::Display(byte w, byte h, TwoWire *wire)
    : Adafruit_SSD1306(w, h, wire) {
    turnedOn = true;
}

/*!
    @brief  Initializes display. Sets text size, color, cursor. 
    @param  vcs
            VCC selection. Pass SSD1306_SWITCHCAPVCC to generate the display
            voltage (step up) from the 3.3V source, or SSD1306_EXTERNALVCC
            otherwise. Most situations with Adafruit SSD1306 breakouts will
            want SSD1306_SWITCHCAPVCC.
    @param  addr
            I2C address of corresponding SSD1306 display (or pass 0 to use
            default of 0x3C for 128x32 display, 0x3D for all others).
            SPI displays (hardware or software) do not use addresses, but
            this argument is still required (pass 0 or any value really,
            it will simply be ignored). Default if unspecified is 0.
    @return Returns true on success. 
*/
void Display::begin(uint8_t vcs, uint8_t addr){
  if(!Adafruit_SSD1306::begin(vcs, addr)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  Adafruit_SSD1306::clearDisplay();
  Adafruit_SSD1306::setTextSize(1);
  Adafruit_SSD1306::setTextColor(SSD1306_WHITE);
  Adafruit_SSD1306::setCursor(0, 0);
  Adafruit_SSD1306::cp437(true);
}

/*!
    @brief  Draws inline emoji for heat index. 
    @param  HI
            Heat index value used to choose emoji.
*/
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

/*!
    @brief  Displays measurements if display is turned ON. 
    @param  POST_INTERVAL
            Time between two POST calls saving measurements. 
            Used to calculate progress bar.
    @param  millisCurrent
            Current value of millis for given loop() iteration.
            Used to calculate progress bar.
    @param  millisLastPost
            Millis of last POST call to api.
            Used to calculate progress bar.
    @param  temperature
            Temperature value
    @param  humidity
            Relative humidity value
    @param  air_pressure
            Air pressure value in hPa
    @param  luminosity
            Luminosity value
    @param  movement
            Movement value
    @param  heatIndex
            Heat index value
    @return Returns true on success. 
*/
void Display::displayMeasurements(
    unsigned int POST_INTERVAL,
    unsigned long millisCurrent,
    unsigned long millisLastPost,
    float temp,
    float RH,
    float HI,
    unsigned int pressure,
    unsigned int lightLevel,
    unsigned int movement
  ){
    if (!turnedOn){
      Display::clear();
      Adafruit_SSD1306::display();
      return;
  }
  
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

/*!
  @brief  Clears display and resets cursor to 0,0
*/
void Display::clear() {
  Adafruit_SSD1306::clearDisplay();
  Adafruit_SSD1306::setCursor(0,0); 
}

/*!
  @brief  Toggles screen on and off
  @return Returns current screen state.
          True is ON, false is OFF.
*/
bool Display::toggleScreen() {
  turnedOn = !turnedOn;

  if (!turnedOn){
    Display::clear();
    Adafruit_SSD1306::display();
  }

  return turnedOn;
}