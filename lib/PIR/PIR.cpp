#include <PIR.h>

PIR::PIR(byte pin) {
    this->pin = pin;
    init();
}

void PIR::init() {
    pinMode(pin, INPUT);
}

byte PIR::read() {
    return digitalRead (pin) == HIGH ? 1 : 0;
}