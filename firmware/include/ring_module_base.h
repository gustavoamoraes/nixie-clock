#pragma once

#include "profile.h"

class RingModule
{
    public:
    
        bool isActive;
        Profile profile;

        void update () {};
        void change (int q) {};
        void back () {};
        void select () {};

        Profile* getProfile() { return &profile; };

        RingModule () {};
};