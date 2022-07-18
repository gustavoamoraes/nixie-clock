#pragma once

#include "constants.h"

class Digit 
{
    public:

        enum TimingFunction { Constant, Ocilating };
        TimingFunction timeFunction;

        void setDuty(float t, TimingFunction type);
        void setDigit(int d);
        
        int getDigit() { return displayDigit; }
        float getDuty();

        Digit ();

    private:
        float baseDuty;
        int displayDigit;
};