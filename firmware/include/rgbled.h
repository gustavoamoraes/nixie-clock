#pragma once

struct RGBColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class RgbLed 
{
    public:

        enum TimingFunction { Constant, Ocilating };
        TimingFunction timeFunction;
        
        RGBColor color;

        void setColor(const RGBColor& color, TimingFunction type);
        void getColor(RGBColor& outColor);

        RgbLed ();
};