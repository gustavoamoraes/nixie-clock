#include <ArduinoJson.h>
#include "Arduino.h"

#include <algorithm>
#include <iterator>

#include "LITTLEFS.h"
#include "constants.h"
#include "rgbled.h"
#include "config.h"

template<typename T>
bool inBounds (T a, T b, T value)
{
    return (value >= a) & (value <= b);
}

RGBColor validateJsonColor (JsonArray& color)
{   
    if (color.size() != 3)
        throw "Color has more or less than 3 values.";

    bool r = inBounds(0, 255, color[0].as<int>());
    bool g = inBounds(0, 255, color[1].as<int>());
    bool b = inBounds(0, 255, color[2].as<int>());

    if (r & g & b)
        return {color[0], color[1], color[2]};
    else throw "Color values are out bounds.";
}

Config::Config (String json)
{
    DynamicJsonDocument doc(512);
    deserializeJson(doc, json);
    
    if( doc.containsKey("digits_brightness"))
    {
        float digitsPwm = doc["digits_brightness"];

        if(inBounds(0.0f, 1.0f, digitsPwm))
            m_DigitsPwm = digitsPwm; 
        else throw "PWM value out of bounds.";
    }

    if( doc.containsKey("led_ring_brightness"))
    {
        float ringBrightness = doc["led_ring_brightness"];

        if(inBounds(0.0f, 1.0f, m_RingBrightness))
            m_RingBrightness = ringBrightness; 
        else throw "Ring brightness value out of bounds.";
    }

    if( doc.containsKey("to_idle"))
        m_ToIdle = (bool) doc["to_idle"];

    if( doc.containsKey("time_to_idle"))
    {
        int idleTime = doc["time_to_idle"];

        if(idleTime > 0) 
            m_TimeToIdle = idleTime;
        else throw "Time to idle is negative.";
    }
    
    if( doc.containsKey("background_color"))
    {
        JsonArray bgColorJson = doc["background_color"];
        RGBColor bgColor = validateJsonColor(bgColorJson);
        m_BgColor = bgColor;
    }
}

template <typename T>
void Config::serialize (T& output)
{
    DynamicJsonDocument jsonConfig(512);

    jsonConfig["background_color"][0] = m_BgColor.r; 
    jsonConfig["background_color"][1] = m_BgColor.g; 
    jsonConfig["background_color"][2] = m_BgColor.b;
    jsonConfig["digits_brightness"] = m_DigitsPwm;
    jsonConfig["led_ring_brightness"] = m_RingBrightness;
    jsonConfig["to_idle"] = m_ToIdle;
    jsonConfig["time_to_idle"] = m_TimeToIdle;

    serializeJson(jsonConfig, output);
}

Config::Config () : m_DigitsPwm(1.0f), m_BgColor({255,0,255})
{
}

bool loadConfigFile()
{
    File configFile = LITTLEFS.open(CONFIG_FILE_PATH, "r");;

    if(!configFile)
        return false;

    String data = configFile.readString();

    Config(data).setAsGlobal();
    configFile.close();
    return true;
}

bool saveGlobalConfig()
{
    File file = LITTLEFS.open(CONFIG_FILE_PATH, "w");
    Config::globalConfig.serialize(file);
    file.close();
}

Config Config::globalConfig;