#pragma once

#include "profile.h"
#include "Arduino.h"

class RingModule
{
    public:
    
        bool isActive = false;

        virtual void update () {};
        virtual void change (int q) {};
        virtual void back () { isActive = false; };
        virtual void select () {};

        virtual Profile* getProfile() { return &m_Profile; };

        RingModule () {};
        ~RingModule () {};

    protected:
        Profile m_Profile;
};