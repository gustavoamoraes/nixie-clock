#include <Arduino.h>

#include "ring_module_clock.h"
#include "globals.h"

void RingModuleClock::update () 
{
    int time[NIXIE_COUNT] {
    g_Datetime.hour()/10, g_Datetime.hour()%10,
    g_Datetime.minute()/10, g_Datetime.minute()%10,
    g_Datetime.second()/10, g_Datetime.second()%10};
    profile.setDigits(time);
}