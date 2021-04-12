#include <GY30.h>

GY30::GY30()
  : BH1750() {}

bool GY30::begin() {
  if (!BH1750::begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("GY30 connection error - check connection!");
    return false;
  }
}