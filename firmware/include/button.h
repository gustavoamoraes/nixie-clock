#pragma once

class Button
{   
    public:

        int pin;
        int wait;
        unsigned long lastRead;
        bool lastState;
        bool lastStableState;
        bool changed;
        bool invert;

        Button (int pin, int wait, bool invert);
        void update ();
        bool wasPressed ();
        bool wasRealised ();
        bool changedA();

};