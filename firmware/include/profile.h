#pragma once

#include "digit.h"
#include "rgbled.h"
#include "constants.h"

struct Profile
{
    Digit nixies[NIXIE_COUNT];
    RgbLed ledRing[LED_RING_COUNT];
    int setDigits (const char* digits);
};