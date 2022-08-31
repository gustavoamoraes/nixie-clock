#include <cmath>
#include <Arduino.h>

#include "rgbled.h"
#include "constants.h"

RgbLed::RgbLed () {}

void RgbLed::setColor (const RGBColor& newColor, TimingFunction type)
{   
    this->color.r = newColor.r;
    this->color.g = newColor.g;
    this->color.b = newColor.b;

    timeFunction = type;
}

void RgbLed::getColor (RGBColor& outColor)
{
    outColor.r = this->color.r;
    outColor.g = this->color.g;
    outColor.b = this->color.b;

    if (timeFunction == TimingFunction::Ocilating){
        float t = (std::cos(PI * 2 * (millis()/1000.0f * OCILATING_PERIOD)) + 1) / 2;

        outColor.r *= t;
        outColor.g *= t;
        outColor.b *= t;
    }
}