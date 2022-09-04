#include "modifiers.h"
#include "Arduino.h"

float positiveSinWave (float n, float period, float t)
{
    return n * (std::sin(PI * 2 * (t * period)) + 1) / 2;
}

RGBColor positiveSinWave (RGBColor color, float period)
{
    float t = millis()/1000.0f;
    return 
    {
        positiveSinWave(color.r, period, t),
        positiveSinWave(color.g, period, t),
        positiveSinWave(color.b, period, t)
    };
}

std::function<float(float)> Ocilation (float period)
{
    return [period] (float duty) { return positiveSinWave(duty, period, millis()/1000.0f); } ;
}

std::function<RGBColor(RGBColor)> ColorOcilation (float period)
{
    return [period] (RGBColor color) { return positiveSinWave(color, period); } ;
}