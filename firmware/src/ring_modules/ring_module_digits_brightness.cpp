#include "ring_module_digits_brightness.h"
#include "constants.h"
#include "modifiers.h"
#include "config.h"

void RingModuleDigitsBrightness::change(int q)
{
    m_Value += q;
    m_Value = min(max(0, m_Value), m_MaxValue);
    m_Ratio = (float) m_Value/m_MaxValue;
    Config::globalConfig.m_DigitsPwm = m_Ratio;

    for (size_t i = 0; i <  LED_RING_COUNT; i++)
    {   
        m_Profile.ledRing[i].setColor(m_LowColor);

        if(i < (m_Ratio*LED_RING_COUNT))
            m_Profile.ledRing[i].setColor(m_Color);
    }

    int number = m_Ratio * 100; 
    m_Profile.displayNumber(number);
}

void RingModuleDigitsBrightness::back()
{
    saveGlobalConfig();
    isActive = false;
}