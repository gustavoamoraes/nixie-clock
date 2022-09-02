#include <Arduino.h>

#include "ring_module_test.h"
#include "modifiers.h"
#include "constants.h"

RingModuleTest::RingModuleTest() : lastMillis(0)
{   
    RGBColor color {0, 255, 255};
    
    for (size_t i = 0; i < LED_RING_COUNT; i++)
    {
        m_Profile.ledRing[i].setColor(color);
        m_Profile.ledRing[i].setModifier(DefaultColorOcilation);
    }
    
}
void RingModuleTest::update () 
{
    if(millis() - lastMillis > 500)
    {
        int digits[NIXIE_COUNT];

        for (size_t j = 0; j < NIXIE_COUNT; j++)
        {
            digits[j] = currentNumber;
        }

        m_Profile.setDigits(digits);
        currentNumber = (currentNumber + 1) % 10;
        lastMillis = millis();
    }
}