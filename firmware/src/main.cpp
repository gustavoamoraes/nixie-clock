#include <Arduino.h>
#include <cmath>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include "NTPClient.h"
#include <WiFi.h>

#include "constants.h"
#include "rotary_encoder.h"
#include "button.h"
#include "profile.h"
#include "ring_module_main.h"
#include "profile.h"
#include "mulitplexer.h"

MultiplexChannel* ringChannel;
Adafruit_NeoPixel pixels(NEO_PIXEL_COUNT, NEO_PIXELS_PIN, NEO_GRB + NEO_KHZ800);

//Inputs
Button backButton(13, 50, true);
Button selectButton(13, 50, true);
RotaryEncoder encoder(13,14);

RTC_PCF8563 rtc;
DateTime g_Datetime;

RingModuleMain mainModule;

WiFiUDP udp;
NTPClient ntpClient = NTPClient(udp);

void initPins ()
{
  // pinMode(13, INPUT_PULLUP);
  // pinMode(13, INPUT_PULLDOWN);
  // pinMode(14, INPUT_PULLDOWN);
}

void updateGlobalDatetime (){
  g_Datetime = rtc.now();
}

void setBackgroundColor (int (&bgColor) [3])
{
  for (size_t i = 0; i < NEO_PIXEL_COUNT; i++)
  {
    pixels.setPixelColor(i, Adafruit_NeoPixel::Color(bgColor[0], bgColor[1], bgColor[2]));
  }
}

void updateDigits (Digit* nixies)
{
  int digitsData;

  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    digitsData |= nixies[i].displayDigit << (i * 4);
    ledcWrite(0, nixies[0].getDuty() * PWM_MAX_VALUE);
  }

  //Each nixie tube requires 4 bits 
  for (size_t i = 0; i < NIXIE_COUNT*4/8; i++)
  {
    digitalWrite(DIGITS_REGISTER_CS, LOW);
    shiftOut(DIGITS_REGISTER_DATA, DIGITS_REGISTER_CLK, MSBFIRST, (digitsData >> i*8));
    digitalWrite(DIGITS_REGISTER_CS, HIGH); 
  }
}

void updateRingColors(RgbLed* ledRing)
{
  for (size_t i = 0; i < LED_RING_COUNT; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      int colorOut[3] ;
      ledRing[i].getColor(colorOut);
      ringChannel->values[i+j] = colorOut[j];
    }
  }
}

void applyProfile(Profile* profile)
{
  updateRingColors(profile->ledRing);
  updateDigits(profile->nixies);
}

// void wifiConnect()
// {
//   WiFi.mode(WIFI_STA);
//   // WiFi.begin(MY_WiFi_SSID, MY_WiFi_PASSWORD);

//   while (WiFi.status() != WL_CONNECTED) 
//   {
//     delay(2);
//   }
// }

void setTimeByNTP (RTC_PCF8563& rtc)
{
  ntpClient.update();
  uint32_t epochTime = ntpClient.getEpochTime();
  DateTime now = DateTime(epochTime) - /*TimeZone*/ TimeSpan(0, 3, 0, 0);
  rtc.adjust(now);
}

void setup() {

  // Serial.begin(115200);

  // initPins ();

  // rtc.writeSqwPinMode(Pcf8563SqwPinMode::PCF8563_SquareWave1Hz);
  // attachInterrupt(digitalPinToInterrupt(RTC_SQW_PIN), &updateGlobalDatetime, RISING);

  // ringChannel = multiplexChannelFactory(10, 3);

  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(NIXIE_PINS[i], i);
  }
}

void loop()
{
  if(backButton.wasPressed())
    mainModule.back();

  if(selectButton.wasPressed())
    mainModule.select();

  int dir = encoder.direction();
  if(dir != 0) mainModule.change(dir);

  mainModule.update();

  applyProfile(mainModule.getProfile());
} 