#pragma once

#include "RTClib.h"
#include "digit.h"
#include "rgbled.h"
#include "config.h"
#include "constants.h"

extern DateTime globalDatetime;
extern Config globalConfig;

void globalSetConfig(Config& newConfig);