#ifndef GY30_H
#define GY30_H

#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>

class GY30 : public BH1750 {
    public:
        GY30();
        void begin();
};

#endif