#pragma once

#include "ring_module_base.h"
#include "ring_module_clock.h"
#include "ring_module_test.h"

class RingModuleMain : public RingModule
{
    public:
        static const int m_ChildCount = 2;
        RingModule* m_Childs[m_ChildCount] { new RingModuleClock(), new RingModuleTest()};

        void update () override;
        void change (int q) override;
        void back () override;
        void select () override;

        Profile* getProfile() override;
        
        RingModuleMain ();

    private:
        RingModule* m_IndexChild;

        const RGBColor m_OtherColor {128, 32, 32};

        const RGBColor m_ChildMenuColors[m_ChildCount] 
        {
            {255,0,0},
            {0,255,0}
        };

        int m_ChildIndex;
};