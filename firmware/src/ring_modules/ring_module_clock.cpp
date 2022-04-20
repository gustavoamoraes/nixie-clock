#include <Arduino.h>

#include "ring_module_clock.h"

void RingModuleClock::update () 
{
    profile.setDigits("123456 c");
}

void RingModuleClock::change (int q) 
{
    test += q;
    Serial.print(test);
}

void RingModuleClock::back () 
{
    isActive = false;
}

void RingModuleClock::select () 
{
    Serial.print("select from clock");
}