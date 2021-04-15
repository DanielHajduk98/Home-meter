#ifndef API_H
#define API_H

#include <Arduino.h>

class API {
    private:
        String url;

    public:
        API(String url);

        int setup();

        void sendMeasurements(
            float temperature,
            float humidity,
            float air_pressure,
            float lumionsity,
            byte movement,
            float heatIndex);
};

#endif