#include <Arduino.h>
#include "button.h"

Button::Button (int pin, int wait, bool invert)
{
    this->pin = pin;
    this->wait = wait;
    this->invert = invert;
}

void Button::update (){
    uint32_t ms = millis();
    changed = false;
    bool currentState = digitalRead(pin) == !invert;
    
    if(currentState != lastState){
        lastState = currentState;
        lastRead = ms;
    }

    if(ms - lastRead > wait){
        if(currentState != lastStableState){
            changed = true;
            lastStableState = currentState;
        }
    }
}

bool Button::wasPressed (){
    return lastStableState && changed;
}

bool Button::wasRealised (){
    return !lastStableState && changed;
}