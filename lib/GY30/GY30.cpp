#include <GY30.h>

GY30::GY30()
  : BH1750() {}

bool GY30::begin() {
  return BH1750::begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}