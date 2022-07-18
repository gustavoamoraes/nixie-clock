#include <Arduino.h>
#include <cmath>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include "NTPClient.h"
#include <WiFi.h>
#include <iostream>
#include "SPI.h"
#include "Wire.h"

#include "constants.h"
#include "rotary_encoder.h"
#include "button.h"
#include "profile.h"
#include "ring_module_main.h"
#include "profile.h"
#include "mulitplexer.h"
#include "globals.h"

MultiplexChannel* ringChannel;
Adafruit_NeoPixel pixels(NEO_PIXEL_COUNT, NEO_PIXELS_PIN, NEO_GRB + NEO_KHZ800);

//Inputs
Button backButton(13, 50, true);
Button selectButton(13, 50, true);
RotaryEncoder encoder(13,14);

//Time
RTC_PCF8563 rtc;
DateTime g_Datetime;

// RingModuleMain mainModule;
RingModuleClock clockModule;

//WiFi
WiFiUDP udp;
NTPClient ntpClient = NTPClient(udp);

SPIClass* digitsRegister = NULL;

int lastMillis = 0;

void initPins ()
{
  pinMode(DIGITS_REGISTER_CS, OUTPUT);  
  pinMode(RTC_SQW_PIN, INPUT_PULLUP);
}

void updateGlobalDatetime ()
{
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
  //updateRingColors(profile->ledRing);
  updateDigits(profile->nixies);
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

void setup() 
{
  // Serial.begin(115200);
  initPins ();

  // attachInterrupt(digitalPinToInterrupt(RTC_SQW_PIN), updateGlobalDatetime, RISING );

  TwoWire* twoWire = new TwoWire(1);
  twoWire->begin(26,25,100000);

  if(!rtc.begin(twoWire))
  {
    // Serial.println("RTC didnt begin");
  }

  rtc.start();
  //rtc.writeSqwPinMode(Pcf8563SqwPinMode::PCF8563_SquareWave1Hz);
  
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
  applyProfile(&clockModule.profile);

  wifiConnect();
  setTimeByNTP();

  int a[3] {255, 0, 255};
  setBackgroundColor(a);
  // ringChannel = multiplexChannelFactory(10, 3);
}

void loop()
{
  if(millis() - lastMillis > 1000)
  {
    g_Datetime = rtc.now();
    lastMillis = millis();
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