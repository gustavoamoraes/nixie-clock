#pragma once

#include "rgbled.h"

struct Config
{
    float digitsPwm = 1.0f;
    RGBColor bgColor {0, 255, 255};
};

bool configFromJson(String json, Config& output);
