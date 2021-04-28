#include <GY30.h>

/*!
  @brief  Extended BH1750 constructor
*/
GY30::GY30()
  : BH1750() {}


/*!
  @brief  Extended BH1750 begin function.
          Sets CONTINUOUS_HIGH_RES_MODE.
*/
bool GY30::begin() {
  return BH1750::begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}