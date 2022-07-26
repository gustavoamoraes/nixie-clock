#pragma once

struct Config
{
    float digitsPwm = 1.0f;
    int bgColor[3] {0, 255, 255};
};

bool configFromJson(String json, Config& output);
