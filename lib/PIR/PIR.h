#ifndef PIR_H
#define PIR_H

#include <Arduino.h>

class PIR {
    private:
        byte pin;

    public:
        PIR(byte pin);
        void init();
        unsigned int millisLast;
        byte read();
};

#endif