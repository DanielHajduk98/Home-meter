#ifndef API_H
#define API_H

#include <Arduino.h>

class API {
    private:
        String url;
    
    public:
        API(String url);
        void sendMeasurements(
            float temperature,
            float humidity,
            float air_pressure,
            float lumionsity,
            bool movement);
};

#endif