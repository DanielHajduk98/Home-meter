#ifndef API_H
#define API_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

class API {
    private:
        String baseUrl;
        
        struct apiCallResult {
            int httpCode;
            String response;
        };

        void saveToken(int addrOffset, const String &token);
        String readToken(int addrOffset);
        apiCallResult POST(String endpoint, String payload, int timeout = 5000);

    public:
        API(String baseUrl);

        int setup();
        
        int sendMeasurements(
            float temperature,
            float humidity,
            float pressure,
            float luminosity,
            byte movement,
            float heatIndex);
};

#endif