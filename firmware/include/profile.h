#pragma once

#include "digit.h"
#include "rgbled.h"
#include "constants.h"

struct Profile
{
    Digit nixies[NIXIE_COUNT];
    RgbLed ledRing[LED_RING_COUNT];
    void setDigits (const int (&new_digits)[NIXIE_COUNT]);
    void displayNumber (size_t number);
};