#include <Arduino.h>

#include "ring_module_test.h"
#include "globals.h"

void RingModuleTest::update () 
{
    if(millis() - lastMillis > 500)
    {
        int digits[NIXIE_COUNT];

        for (size_t j = 0; j < NIXIE_COUNT; j++)
        {
            digits[j] = currentNumber;
        }

        profile.setDigits(digits);
        currentNumber = (currentNumber + 1) % 10;
        lastMillis = millis();
    }
}