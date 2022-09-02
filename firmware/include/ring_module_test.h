#pragma once

#include "ring_module_base.h"

class RingModuleTest : public RingModule
{
    public:
        void update () override;
        
        RingModuleTest ();
    private:
        int myCounter;
        int currentNumber;
        int lastMillis;
};