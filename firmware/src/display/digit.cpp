#include <cmath>
#include <Arduino.h>

#include "digit.h"
#include "constants.h"
#include "globals.h"

void Digit::setTimeFunction (TimingFunction type){   
    timeFunction = type;
}

float Digit::getDuty (){
    if (timeFunction == TimingFunction::Ocilating)
        return globalConfig.digitsPwm * (std::cos(PI * 2 * (millis()/1000.0f * OCILATING_PERIOD)) + 1) / 2;
    return globalConfig.digitsPwm;
}

void Digit::setDigit(int d){
    displayDigit = std::max(0, std::min(d, 9));
}