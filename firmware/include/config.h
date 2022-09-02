#pragma once

#include <Arduino.h>
#include "rgbled.h"

class Config
{
    public:
        static Config globalConfig;

        Config ();
        Config(String json);

        template <typename T>
        void serialize (T& output);

        void setAsGlobal () { Config::globalConfig = *this; };

        float m_DigitsPwm = 1.0f;
        float m_RingBrightness = 1.0f;
        RGBColor m_BgColor {0, 255, 255};
        unsigned int m_TimeToIdle = 15;
        bool m_ToIdle = true;
};

bool loadConfigFile();
bool saveGlobalConfig();