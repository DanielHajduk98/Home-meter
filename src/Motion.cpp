#define MOTION_PIR D5
    
#include <Arduino.h>

// This sensor needs 1 min to warm up!

void motionSetup() {
    pinMode(MOTION_PIR, INPUT);
}
    
byte getMovement() {
    return digitalRead (MOTION_PIR) == HIGH ? 1 : 0;
}