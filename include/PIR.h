#ifndef PIR_H
#define PIR_H

#include <Arduino.h>

class PIR {
    private:
        byte pin;
        void init();

    public:
        PIR(byte pin);
        unsigned int millisLast;
        byte read();
};

#endif