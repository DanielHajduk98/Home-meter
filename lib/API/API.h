#ifndef API_H
#define API_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

class API {
    private:
        String url;
        
        void writeStringToEEPROM(int addrOffset, const String &strToWrite);
        String readStringFromEEPROM(int addrOffset);
        
    public:
        API(String url);

        int setup();
        
        int sendMeasurements(
            float temperature,
            float humidity,
            float air_pressure,
            float lumionsity,
            byte movement,
            float heatIndex);
};

#endif