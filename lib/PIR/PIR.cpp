#include <PIR.h>

/*!
    @brief  PIR class constructor
    @param  pin
            Pin which is connected to PIR
*/
PIR::PIR(byte pin) {
    this->pin = pin;
}


/*!
    @brief  Sets pir pin into INPUT mode
*/
void PIR::init() {
    pinMode(pin, INPUT);
}

/*!
    @brief  Returns value from pir.
    @return returns 1 when movement detected. 
*/
byte PIR::read() {
    return digitalRead (pin) == HIGH ? 1 : 0;
}