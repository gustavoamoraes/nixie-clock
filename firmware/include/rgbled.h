#pragma once

#include <inttypes.h>
#include <functional>

struct RGBColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class RgbLed 
{
    public:
        void setColor(RGBColor newColor) { m_Color = newColor;};
        void setModifier(std::function<RGBColor(RGBColor)> modifier) { m_Modifier = modifier; };
        RGBColor getColor() { return m_Modifier != NULL ? m_Modifier(m_Color) : m_Color; };

        RgbLed () {};
        ~RgbLed () {};

    private:
        RGBColor m_Color;
        std::function<RGBColor(RGBColor)> m_Modifier;
};