#ifndef BMP280_H
#define BMP280_H

#include <Arduino.h>
#include <Adafruit_BMP280.h>

class BMP280 : public Adafruit_BMP280 {
    private: 

    public:
        BMP280();

        float pressure;
        float temp;

        void begin();
        float getPressure();
        float getTemp(bool = false);
};

#endif