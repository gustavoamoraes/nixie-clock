#pragma once

class RgbLed 
{
    public:

        enum TimingFunction { Constant, Ocilating };
        TimingFunction timeFunction;
        
        int color[3];

        void setColor(int (&rgb)[3], TimingFunction type);
        void getColor(int (&output) [3]);

        RgbLed ();
};