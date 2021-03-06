#ifndef PIR_H
#define PIR_H

#include <Arduino.h>

class PIR {
    private:
        byte pin;
    
    public:
        PIR(byte pin);
        byte movement;
        unsigned int millisLast;
        void init();
        byte getMovement();
};

#endif