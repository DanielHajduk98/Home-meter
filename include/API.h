#ifndef API_H
#define API_H

#include <Arduino.h>

class API {
    private:
        String url;
        long monitor_id;
        void create();

    public:
        API(String url);
        void sendMeasurements(
            float temperature,
            float humidity,
            float air_pressure,
            float lumionsity,
            byte movement,
            float heatIndex);
};

#endif