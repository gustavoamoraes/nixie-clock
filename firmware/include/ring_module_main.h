#pragma once

#include "ring_module_base.h"
#include "ring_module_clock.h"

class RingModuleMain : public RingModule
{
    public:
        
        static const int childCount = 2;
        RingModule childs[childCount] { RingModuleClock() };
        RingModule* indexChild;
        int childIndex;

        void update ();
        void change (int q);
        void back ();
        void select ();

        Profile* getProfile();
        
        RingModuleMain ();
};