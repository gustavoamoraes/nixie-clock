#pragma once

#include "ring_module_base.h"

class RingModuleTest : public RingModule
{
    public:
        void update () override;
        void change (int q) override {};
        void back () override { isActive = false; };
        void select () override {};

        RingModuleTest () {};

    private:
        int myCounter;
        int currentNumber;
        int lastMillis;
};