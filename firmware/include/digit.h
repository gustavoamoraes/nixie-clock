#pragma once

#include "constants.h"

class Digit 
{
    public:

        enum TimingFunction { Constant, Ocilating };
        TimingFunction timeFunction;

        float baseDuty;
        int displayDigit;

        void setDuty(float t, TimingFunction type);
        void setDigit(int d);
        float getDuty();

        Digit ();
};