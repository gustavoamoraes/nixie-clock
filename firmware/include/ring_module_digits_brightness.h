#pragma once

#include "ring_module_base.h"

class RingModuleDigitsBrightness : public RingModule
{
    public:
        void change (int q) override;
        void back () override;
    private:
        const RGBColor m_Color {255,0,255};
        const RGBColor m_LowColor {0,0,0};
        int m_Value;
        const int m_MaxValue = 20;
        float m_Ratio;
};