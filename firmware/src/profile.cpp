#include <Arduino.h>
#include "profile.h"

void Profile::setDigits (const int new_digits[NIXIE_COUNT])
{
  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    nixies[i].setDigit(new_digits[i]);
  }
}