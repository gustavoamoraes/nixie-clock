#pragma once

#include <functional>
#include "constants.h"
#include "config.h"

class Digit 
{
    public:
        void setDigit(int d) { m_DisplayDigit = std::max(0, std::min(d, 9)); };   
        void setModifier(std::function<float(float)> modifier) { m_Modifier = modifier; };   
        int getDigit() { return m_DisplayDigit; }
        
        float getDuty() 
        { 
            return (m_Modifier != NULL) ? m_Modifier(Config::globalConfig.m_DigitsPwm) : Config::globalConfig.m_DigitsPwm; 
        };

        Digit () {};
        ~Digit () {};

    private:
        int m_DisplayDigit;
        std::function<float(float)> m_Modifier;
};