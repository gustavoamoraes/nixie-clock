#include <Arduino.h>
#include <cmath>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include "NTPClient.h"
#include <WiFi.h>
#include <iostream>
#include "SPI.h"

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

unsigned long lastTime = 0;
int count = 0;

static const int spiClk = 1000000; // 1 MHz
SPIClass * vspi = NULL;

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
  unsigned long digitsData = 0;

  for (int i = (NIXIE_COUNT - 1); i >= 0; i--)
  {
    digitsData |= nixies[i].displayDigit << (i * 4);
    ledcWrite(i, nixies[i].getDuty() * PWM_MAX_VALUE);
  }

  //use it as you would the regular arduino SPI API
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(DIGITS_REGISTER_CS, LOW); //pull SS slow to prep other end for transfer
  vspi->transfer32(digitsData);  
  digitalWrite(DIGITS_REGISTER_CS, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();

  /*vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(DIGITS_REGISTER_CS, LOW); //pull SS slow to prep other end for transfer

  //Each nixie tube requires 4 bits 
  for (size_t i = 0; i < NIXIE_COUNT*4/8; i++)
  {
    vspi->transfer(digitsData >> i*8);  
  }
  
  digitalWrite(DIGITS_REGISTER_CS, HIGH);
  vspi->endTransaction();*/
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
  //SPI.setClockDivider(SPI_CLOCK_DIV128);

  vspi = new SPIClass(VSPI);
  vspi->begin(DIGITS_REGISTER_CLK, -1, DIGITS_REGISTER_DATA, DIGITS_REGISTER_CS);

  pinMode(DIGITS_REGISTER_CS, OUTPUT);  
  digitalWrite(DIGITS_REGISTER_CS, HIGH);  

  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(NIXIE_PINS[i], i);
    mainModule.profile.nixies[i].setDigit(i);
  }
}

void loop()
{
  uint32_t diff = millis() - lastTime;

  if( diff > 1000)
  {
    for (size_t i = 0; i < NIXIE_COUNT; i++)
    {
      mainModule.profile.nixies[i].setDigit((count + i) % 10); 
    }

    count++;

    lastTime = millis();
  }

  /* if(backButton.wasPressed())
    mainModule.back();

  if(selectButton.wasPressed())
    mainModule.select();

  int dir = encoder.direction();
  if(dir != 0) mainModule.change(dir); */

  //mainModule.update();
  applyProfile(mainModule.getProfile());
} 