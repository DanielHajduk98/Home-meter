#include <GY30.h>

GY30::GY30()
  : BH1750() {}

void GY30::begin() {
  if (BH1750::begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }
}