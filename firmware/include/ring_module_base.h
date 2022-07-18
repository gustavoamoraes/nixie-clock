#pragma once

#include "profile.h"

class RingModule
{
    public:
    
        bool isActive;
        Profile profile;

        virtual void update () {};
        virtual void change (int q) {};
        virtual void back () {};
        virtual void select () {};

        virtual Profile* getProfile() { return &profile; };

        RingModule () {};
};