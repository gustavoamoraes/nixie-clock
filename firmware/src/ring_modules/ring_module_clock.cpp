#include <Arduino.h>

#include "ring_module_clock.h"
#include "globals.h"

RingModuleClock::RingModuleClock () 
{
    for (size_t i = 0; i < LED_RING_COUNT; i++)
    {
        m_Profile.ledRing[i].setColor(m_Color);
    }
}

void RingModuleClock::update () 
{
    int time[NIXIE_COUNT] {
    globalDatetime.hour()/10, globalDatetime.hour()%10,
    globalDatetime.minute()/10, globalDatetime.minute()%10,
    globalDatetime.second()/10, globalDatetime.second()%10};
    m_Profile.setDigits(time);
}