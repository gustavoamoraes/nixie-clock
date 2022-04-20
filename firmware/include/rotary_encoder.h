#pragma once

class RotaryEncoder
{   
    int dt;
    int clk;
    int lastClk;
    
    public:
        RotaryEncoder (int dtPin, int clkPin);
        int direction ();
};