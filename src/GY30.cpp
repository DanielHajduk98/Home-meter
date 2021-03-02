#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);

void GY30Begin(){
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }

}

float getLx() {
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    return lux;
  }
}