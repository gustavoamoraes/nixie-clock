#include <Arduino.h>

#include "ring_module_clock.h"
#include "globals.h"


RingModuleClock::RingModuleClock () 
{
    for (size_t i = 0; i < LED_RING_COUNT; i++)
    {
        profile.ledRing[i].setColor(m_Color, RgbLed::TimingFunction::Ocilating);
    }
}

void RingModuleClock::change (int q)
{
    m_Color.r = (255 + ((m_Color.r + q) % 255)) % 255;

    Serial.println(m_Color.r);
    
    for (size_t i = 0; i < LED_RING_COUNT; i++)
    {
        profile.ledRing[i].setColor(m_Color, RgbLed::TimingFunction::Constant);
    }
};

void RingModuleClock::update () 
{
    int time[NIXIE_COUNT] {
    globalDatetime.hour()/10, globalDatetime.hour()%10,
    globalDatetime.minute()/10, globalDatetime.minute()%10,
    globalDatetime.second()/10, globalDatetime.second()%10};
    profile.setDigits(time);
}