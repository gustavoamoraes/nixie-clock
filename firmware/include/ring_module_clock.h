#pragma once

#include <RTClib.h>
#include "ring_module_base.h"
#include "Arduino.h"
#include "rgbled.h"

class RingModuleClock : public RingModule
{
    public:
        void update () override;
        void change (int q) override;
        void back () override { isActive = false; };
        
        void select () override {};
        RingModuleClock ();

    private:
        RGBColor m_Color {0,0,0};
        DateTime m_Datetime;
};