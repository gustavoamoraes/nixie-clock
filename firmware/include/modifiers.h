#pragma once

#include <cmath>
#include "rgbled.h"
#include "constants.h"

float positiveSinWave (float n, float t, float period);
RGBColor positiveSinWave (RGBColor color, float period);

std::function<float(float)> Ocilation (float period);
std::function<RGBColor(RGBColor)> ColorOcilation (float period);

const auto DefaultColorOcilation = ColorOcilation(OCILATING_PERIOD);
const auto DefaultOcilation = Ocilation(OCILATING_PERIOD);
const auto Off = [] (float duty) { return 0.0f; } ;