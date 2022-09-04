#include <Arduino.h>
#include <cmath>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include "NTPClient.h"
#include <WiFi.h>
#include <iostream>
#include "SPI.h"
#include "Wire.h"
#include "SPI.h"
#include "LITTLEFS.h"

#include "constants.h"
#include "rotary_encoder.h"
#include "button.h"
#include "profile.h"
#include "ring_module_main.h"
#include "ring_module_test.h"
#include "profile.h"
#include "globals.h"
#include "config.h"
#include "web.h"
#include "modifiers.h"

#ifdef __cplusplus
  extern "C" {
#endif

#include "mulitplexer.h"

#ifdef __cplusplus
  }
#endif

//
MultiplexChannel* ringChannels[1];
Adafruit_NeoPixel pixels(NEO_PIXEL_COUNT, NEO_PIXELS_PIN, NEO_GRB + NEO_KHZ800);

//Inputs
Button backButton(BACK_BUTTON_PIN, 50, true);
Button selectButton(ENCODER_SW, 50, true);
RotaryEncoder encoder(ENCODER_DT, ENCODER_CLK);

//Time
RTC_DS3231 rtc;
DateTime globalDatetime;

RingModuleMain mainModule;

//WiFi
WiFiUDP udp;
NTPClient ntpClient = NTPClient(udp);

SPIClass* digitsRegister = NULL;
bool canUpdateGlobalTime;
bool isIdle;
uint32_t  lastActiveTime;
uint32_t  lastPixelsUpdateTime;

void initPins ()
{
  pinMode(DIGITS_REGISTER_CS, OUTPUT);  
  pinMode(MULTIPLEXER_CS, OUTPUT);  

  pinMode(ENCODER_DT, INPUT);  
  pinMode(ENCODER_CLK, INPUT);  
  pinMode(ENCODER_SW, INPUT);  
  pinMode(BACK_BUTTON_PIN, INPUT);  
  pinMode(RTC_SQW_PIN, INPUT);
}

void IRAM_ATTR updateGlobalDatetime ()
{
  canUpdateGlobalTime = true;
}

void globalSetBackgroundColor (RGBColor& bgColor)
{
  pixels.fill(Adafruit_NeoPixel::Color(bgColor.r, bgColor.g, bgColor.b));
  pixels.show();
}

void updateDigits (Digit (&nixies)[NIXIE_COUNT])
{
  unsigned long digitsData = 0;

  for (int i = (NIXIE_COUNT - 1); i >= 0; i--)
  {
    int inverseDigit = (NIXIE_COUNT - 1) - i;
    digitsData |= nixies[inverseDigit].getDigit() << (i * 4);
    ledcWrite(i, nixies[inverseDigit].getDuty() * PWM_MAX_VALUE);
  }

  digitsRegister->beginTransaction(SPISettings(SPI_CLK_1MHZ, MSBFIRST, SPI_MODE0));
  digitalWrite(DIGITS_REGISTER_CS, LOW);
  digitsRegister->transfer32(digitsData);  
  digitalWrite(DIGITS_REGISTER_CS, HIGH);
  digitsRegister->endTransaction();
}

void updateRingColors(RgbLed (&leds)[LED_RING_COUNT])
{
  RGBColor colorOut;

  for (size_t i = 0; i < LED_RING_COUNT; i++)
  {
    colorOut = leds[i].getColor();

    bool idle = !(isIdle & Config::globalConfig.m_ToIdle);
    float mult = Config::globalConfig.m_RingBrightness;

    ringChannels[0]->values[(i*3)] = colorOut.r / 255.0f * MULTIPLEXER_COUNTER_MAX * mult * idle;
    ringChannels[0]->values[(i*3)+1] = colorOut.g / 255.0f * MULTIPLEXER_COUNTER_MAX * mult * idle;
    ringChannels[0]->values[(i*3)+2] = colorOut.b / 255.0f * MULTIPLEXER_COUNTER_MAX * mult * idle;
  }
}

void wifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(WiFi_SSID, WiFi_PASSWORD);
    delay(6000);
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
  updateRingColors(profile->ledRing);
}

void mainLoop(void* data)
{
  while(1)
  {
    isIdle = (millis() - lastActiveTime) > (Config::globalConfig.m_TimeToIdle * 1000);

    if( canUpdateGlobalTime){
      globalDatetime = rtc.now();
      canUpdateGlobalTime = false;
    }

    backButton.update();
    if(backButton.wasPressed()){
      lastActiveTime = millis();
      mainModule.back();
    }

    selectButton.update();
    if(selectButton.wasPressed()){
      lastActiveTime = millis();
      mainModule.select();
    }

    int dir = encoder.direction();
    
    if(dir != 0){
      lastActiveTime = millis();
      mainModule.change(dir);
    }

    mainModule.update();

    Profile* currentProfile = mainModule.getProfile();
    applyProfile(currentProfile);
    delay(2);
  }
}

//Will be shown while the WiFi connects
void pauseChamp ()
{
  Profile profile;

  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    profile.nixies[i].setDigit(9);
  }

  RGBColor ringColor {255, 0, 64};

  for (size_t i = 0; i < LED_RING_COUNT; i++)
  {
    profile.ledRing[i].setColor(ringColor);
  }

  RGBColor bgColor {255, 0, 255};
  globalSetBackgroundColor(bgColor);
  applyProfile(&profile);
}

void setup() 
{
  Serial.begin(115200);
  initPins ();

  //Pixels
  pixels.begin();

  //Digits
  digitsRegister = new SPIClass(VSPI);
  digitsRegister->begin(DIGITS_REGISTER_CLK, -1, DIGITS_REGISTER_DATA, DIGITS_REGISTER_CS);
  digitalWrite(DIGITS_REGISTER_CS, HIGH);

  //PWM
  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(NIXIE_PINS[i], i);
  } 

  //LED Ring
  ringChannels[0] = multiplexChannelFactory(LED_RING_COUNT, 3);
  multiplexer_init(ringChannels, 1, 2);

  pauseChamp();

  //RTC
  TwoWire* twoWire = new TwoWire(1);
  twoWire->begin(26,25,100000);
  rtc.begin(twoWire);

  //WiFi
  wifiConnect();
  setTimeByNTP();

  //Interupt
  rtc.disable32K();
  rtc.writeSqwPinMode(Ds3231SqwPinMode::DS3231_SquareWave1Hz);
  attachInterrupt(digitalPinToInterrupt(RTC_SQW_PIN), updateGlobalDatetime, FALLING);

  //File system
  LITTLEFS.begin();

  //Web page  
  startServer();

  //Apply saved config
  loadConfigFile();

  //Loop
  xTaskCreatePinnedToCore(
      mainLoop,  
      "MainLoop",  
      1024 * 15,           
      NULL,            
      1,               
      NULL,
      0
  );
}

void loop()
{
  vTaskDelete(NULL);
} 