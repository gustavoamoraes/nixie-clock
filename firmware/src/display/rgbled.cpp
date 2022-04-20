#include <cmath>
#include <Arduino.h>

#include "rgbled.h"
#include "constants.h"

RgbLed::RgbLed () {}

void RgbLed::setColor (int (&rgb)[3], TimingFunction type){   
    memcpy(this->color, rgb, 3 * sizeof(int));
    timeFunction = type;
}

void RgbLed::getColor (int (&output) [3] )
{
    memcpy(this->color, output, 3 * sizeof(int));

    if (timeFunction == TimingFunction::Ocilating){
        float t = (std::cos(PI * 2 * (millis()/1000.0f * OCILATING_PERIOD)) + 1) / 2;

        for (size_t i = 0; i < 3; i++){
            output[i] *= t;
        }
    }
}