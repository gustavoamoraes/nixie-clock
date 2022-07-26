#include <ArduinoJson.h>
#include "config.h"

template<typename T>
bool inbounds (T a, T b, T value)
{
    return value >= a & value =< b;
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

    float digitsPwm = doc["digits_brigthness"];

    if(inbounds<float>(0.0f, 1.0f, digitsPwm)) 
        outputConfig.digitsPwm= digitsPwm;
    else return false;

    int* rgb = doc["background_color"]; 

    if(validateColor(rgb))  
        std::copy(rgb[0], rgb[2], outputConfig.bgColor[0]);
    else return false;

    return true;
}