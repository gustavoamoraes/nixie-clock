#include <Arduino.h>
#include <cmath>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include "NTPClient.h"
#include <WiFi.h>
#include <iostream>
#include "SPI.h"
#include "Wire.h"
#include "SPIFFS.h"

#include "constants.h"
#include "rotary_encoder.h"
#include "button.h"
#include "profile.h"
#include "ring_module_main.h"
#include "profile.h"
#include "mulitplexer.h"
#include "globals.h"
#include "config.h"

//
MultiplexChannel* ringChannel;
Adafruit_NeoPixel pixels(NEO_PIXEL_COUNT, NEO_PIXELS_PIN, NEO_GRB + NEO_KHZ800);

//Inputs
Button backButton(13, 50, true);
Button selectButton(13, 50, true);
RotaryEncoder encoder(13,14);

//Time
RTC_DS3231 rtc;
DateTime globalDatetime;

// RingModuleMain mainModule;
RingModuleClock clockModule;

//WiFi
WiFiUDP udp;
NTPClient ntpClient = NTPClient(udp);

SPIClass* digitsRegister = NULL;

Config globalConfig;

int lastMillis = 0;
bool canUpdateGlobalTime;

void initPins ()
{
  pinMode(DIGITS_REGISTER_CS, OUTPUT);  
  pinMode(RTC_SQW_PIN, INPUT);
}

void updateGlobalDatetime ()
{
  canUpdateGlobalTime = true;
}

void setConfig(Config& newConfig)
{
  setBackgroundColor(newConfig.bgColor);
  globalConfig = newConfig;
}

void setBackgroundColor (int (&bgColor) [3])
{
  for (size_t i = 0; i < NEO_PIXEL_COUNT; i++)
  {
    pixels.setPixelColor(i, Adafruit_NeoPixel::Color(bgColor[0], bgColor[1], bgColor[2]));
  }
}

void updateDigits (Digit (&nixies)[NIXIE_COUNT])
{
  unsigned long digitsData = 0;

  for (int i = (NIXIE_COUNT - 1); i >= 0; i--)
  {
    digitsData |= nixies[i].getDigit() << (i * 4);
    ledcWrite(i, nixies[i].getDuty() * PWM_MAX_VALUE);
  }

  digitsRegister->beginTransaction(SPISettings(SPI_CLK_1MHZ, MSBFIRST, SPI_MODE0));
  digitalWrite(DIGITS_REGISTER_CS, LOW);
  digitsRegister->transfer32(digitsData);  
  digitalWrite(DIGITS_REGISTER_CS, HIGH);
  digitsRegister->endTransaction();
}

void updateRingColors(RgbLed (&leds)[LED_RING_COUNT])
{
  for (size_t i = 0; i < LED_RING_COUNT; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      int colorOut[3] ;
      leds[i].getColor(colorOut);
      ringChannel->values[i+j] = colorOut[j];
    }
  }
}

void wifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_SSID, WiFi_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(2);
  }
}

void setTimeByNTP ()
{
  ntpClient.update();
  uint32_t epochTime = ntpClient.getEpochTime();
  DateTime now = DateTime(epochTime) - /*TimeZone*/ TimeSpan(0, 3, 0, 0);
  rtc.adjust(now);
}

void applyProfile (Profile* profile)
{
  updateDigits(profile->nixies);
}

void loadConfigFile()
{
  File file = SPIFFS.open(CONFIG_FILE_PATH, "r");
  Config savedConfig;

  //If file exists
  if(file || !file.isDirectory())
  {
    String data = file.readString();
    configFromJson(data, savedConfig);
  }

  setConfig(savedConfig);
  file.close();
}

void setup() 
{
  //Serial.begin(115200);
  initPins ();

  digitsRegister = new SPIClass(VSPI);
  digitsRegister->begin(DIGITS_REGISTER_CLK, -1, DIGITS_REGISTER_DATA, DIGITS_REGISTER_CS);
  digitalWrite(DIGITS_REGISTER_CS, HIGH);

  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(NIXIE_PINS[i], i);
    clockModule.profile.nixies[i].setDigit(0);
  }  

  //Set all zeros

  TwoWire* twoWire = new TwoWire(1);
  twoWire->begin(26,25,100000);

  if(!rtc.begin(twoWire)) {}

  wifiConnect();
  setTimeByNTP();

  rtc.disable32K();
  rtc.writeSqwPinMode(Ds3231SqwPinMode::DS3231_SquareWave1Hz);
  attachInterrupt(digitalPinToInterrupt(RTC_SQW_PIN), updateGlobalDatetime, FALLING );

  int a[3] {255, 0, 255};
  setBackgroundColor(a);
  // ringChannel = multiplexChannelFactory(10, 3);
}

void loop()
{
  if( canUpdateGlobalTime)
  {
    globalDatetime = rtc.now();
    canUpdateGlobalTime = false;
  }
  /* if(backButton.wasPressed())
    mainModule.back();

  if(selectButton.wasPressed())
    mainModule.select();

  int dir = encoder.direction();
  if(dir != 0) mainModule.change(dir); */

  //mainModule.update();
  // applyProfile(mainModule.getProfile());
  clockModule.update();
  applyProfile(clockModule.getProfile());
} 