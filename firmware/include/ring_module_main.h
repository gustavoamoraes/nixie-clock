#pragma once

#include "ring_module_base.h"
#include "ring_module_clock.h"
#include "ring_module_test.h"
#include "ring_module_digits_brightness.h"

class RingModuleMain : public RingModule
{
    public:
        static const int m_ChildCount = 3;
        RingModule* m_Childs[m_ChildCount] 
        { 
            new RingModuleClock(), 
            new RingModuleDigitsBrightness(),
            new RingModuleTest(),
        };

        void update () override;
        void change (int q) override;
        void back () override;
        void select () override;

        Profile* getProfile() override;
        
        RingModuleMain ();

    private:
        RingModule* m_IndexChild;

        const RGBColor m_ChildMenuColors[m_ChildCount] 
        {
            {255,0,0},
            {0,255,0},
            {0,0,255}
        };

        int m_ChildIndex;
};