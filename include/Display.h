#ifndef Display_H
#define Display_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

class Display : public Adafruit_SSD1306 {
    private:
        void drawHeatIndexEmoji(float HI);

    public:
        Display(byte w, byte H, TwoWire *Wire);
        
        bool turnedOn = true;

        void begin(uint8_t vcs, uint8_t addr);
        void displayMeasurements(unsigned int POST_INTERVAL, unsigned long millisCurrent, unsigned long millisLastPost, float temp, float RH, float HI, unsigned int pressure, unsigned int lightLevel, unsigned int movement);
        void clear();
        void toggleScreen();
};

#endif