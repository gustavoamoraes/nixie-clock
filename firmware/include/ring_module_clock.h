#pragma once

#include <RTClib.h>
#include "ring_module_base.h"

class RingModuleClock : public RingModule
{
    public:
        void update () override;
        void change (int q) override {};
        void back () override { isActive = false; };
        void select () override {};

        RingModuleClock () {};

    private:
        DateTime m_Datetime;
};