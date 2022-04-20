#include <Arduino.h>

#include "rotary_encoder.h"

RotaryEncoder::RotaryEncoder (int dtPin, int clkPin)
{
    this->dt = dtPin;
    this->clk = clkPin;
}

int RotaryEncoder::direction ()
{
    bool clkValue = digitalRead(clk);
    bool dtValue = digitalRead(dt);

    int dir = 0;

    if (clkValue !=  lastClk){
        dir = clkValue && dtValue ? 1 : -1;
        lastClk = clkValue;
    }

    return dir;
}
