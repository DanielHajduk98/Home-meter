#ifndef BMP280_H
#define BMP280_H

#include <Arduino.h>
#include <Adafruit_BMP280.h>

class BMP280 : public Adafruit_BMP280 {
    public:
        BMP280();
        void begin();
};

#endif