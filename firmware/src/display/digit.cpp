#include <cmath>
#include <Arduino.h>

#include "digit.h"
#include "constants.h"

Digit::Digit () : baseDuty(1.0f) 
{};

void Digit::setDuty (float d, TimingFunction type){   
    baseDuty = std::max(0.0f, std::min(d, 1.0f));
    timeFunction = type;
}

float Digit::getDuty (){
    if (timeFunction == TimingFunction::Ocilating)
        return baseDuty * (std::cos(PI * 2 * (millis()/1000.0f * OCILATING_PERIOD)) + 1) / 2;
    return baseDuty;
}

void Digit::setDigit(int d){
    displayDigit = std::max(0, std::min(d, 9));
}