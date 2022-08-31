#pragma once

#include "profile.h"
#include "Arduino.h"

class RingModule
{
    public:
    
        bool isActive;

        virtual void update () {};
        virtual void change (int q) {};
        virtual void back () {};
        virtual void select () { Serial.print("Select form base");};

        virtual Profile* getProfile() { return &profile; };

        RingModule () {};
        ~RingModule () {};

    protected:
        Profile profile;
};