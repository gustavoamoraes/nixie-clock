#pragma once

#include <RTClib.h>
#include "Arduino.h"

#include "ring_module_base.h"
#include "rgbled.h"

class RingModuleClock : public RingModule
{
    public:
        void update () override;
        void back () override { isActive = false; };

        RingModuleClock();

    private:
        RGBColor m_Color {255,128,64};
        DateTime m_Datetime;
};