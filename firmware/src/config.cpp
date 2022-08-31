#include <ArduinoJson.h>
#include "config.h"

#include <algorithm>
#include <iterator>

template<typename T>
bool inbounds (T a, T b, T value)
{
    return (value >= a) & (value <= b);
}

bool validateColor (int* color)
{
    bool r = inbounds<int>(0, 255, color[0]);
    bool g = inbounds<int>(0, 255, color[1]);
    bool b = inbounds<int>(0, 255, color[2]);

    return r & g & b;
}

bool configFromJson(String json, Config& outputConfig)
{
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);

    Config configCopy = outputConfig;

    //PWM
    float digitsPwm = doc["digits_brigthness"];

    if(inbounds<float>(0.0f, 1.0f, digitsPwm)) 
        configCopy.digitsPwm= digitsPwm;
    else return false;

    //Color
    JsonArray rgb = doc["background_color"].as<JsonArray>(); 
    int rgbInt[3] 
    { 
        rgb[0].as<int>(), 
        rgb[1].as<int>(), 
        rgb[2].as<int>() 
    };

    if(validateColor(rgbInt))
        memcpy(&configCopy.bgColor, rgbInt, 3*sizeof(int));
    else return false;

    outputConfig = configCopy;

    return true;
}