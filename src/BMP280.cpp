#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

void BMP280Begin() {
    if (!bmp.begin(0x76)) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
        while (1);
    }
    
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


    Serial.println(F("BMP280 test"));
}

float getTemp(bool inF = false) {
    const float celsius = bmp.readTemperature();
    if(inF) {
        return (celsius * 9.0) / 5.0 + 32;
    }
    return celsius;
}

// In hPa
float getPressure() {
    return (bmp.readPressure()/100);
}