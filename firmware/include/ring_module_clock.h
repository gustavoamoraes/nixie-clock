#pragma once

#include "ring_module_base.h"

class RingModuleClock : public RingModule
{
    public:
    
        int test;

        void update ();
        void change (int q);
        void back ();
        void select ();

        RingModuleClock () {};
};