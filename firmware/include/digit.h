#pragma once

#include "constants.h"

class Digit 
{
    public:
        enum TimingFunction { Constant, Ocilating };
        TimingFunction timeFunction;

        void setTimeFunction(TimingFunction type);
        void setDigit(int d);
        
        int getDigit() { return displayDigit; }
        float getDuty();

        Digit () {};

    private:
        int displayDigit;
};