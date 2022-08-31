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
#include "ring_module_test.h"
#include "profile.h"
#include "globals.h"
#include "config.h"
#include "web.h"

#ifdef __cplusplus
  extern "C" {
#endif

#include "mulitplexer.h"

#ifdef __cplusplus
  }
#endif


//
MultiplexChannel* ringChannel;
MultiplexChannel* ringChannels[1];
Adafruit_NeoPixel pixels(NEO_PIXEL_COUNT, NEO_PIXELS_PIN, NEO_GRB + NEO_KHZ800);

//Inputs
Button backButton(13, 50, true);
Button selectButton(34, 50, true);
RotaryEncoder encoder(ENCODER_DT, ENCODER_CLK);

//Time
RTC_DS3231 rtc;
DateTime globalDatetime;

RingModuleMain mainModule;
//RingModuleTest clockModule;
//RingModuleClock clockModule;

//WiFi
WiFiUDP udp;
NTPClient ntpClient = NTPClient(udp);

SPIClass* digitsRegister = NULL;

Config globalConfig;

int encoderTest = 0;
bool canUpdateGlobalTime;
const int color[3] {125, 0, 0};
const int color2[3] {125, 0, 125};
TaskHandle_t loop_task_handle_test;

void initPins ()
{
  pinMode(DIGITS_REGISTER_CS, OUTPUT);  
  pinMode(MULTIPLEXER_CS, OUTPUT);  

  pinMode(ENCODER_DT, INPUT);  
  pinMode(ENCODER_CLK, INPUT);  
  pinMode(ENCODER_SW, INPUT);  

  pinMode(RTC_SQW_PIN, INPUT);
}

void updateGlobalDatetime ()
{
  canUpdateGlobalTime = true;
}

void setBackgroundColor (RGBColor bgColor)
{
  for (size_t i = 0; i < NEO_PIXEL_COUNT; i++)
  {
    pixels.setPixelColor(i, Adafruit_NeoPixel::Color(bgColor.r, bgColor.g, bgColor.b));
  }

  pixels.show();
}

void globalSetConfig(Config& newConfig)
{
  setBackgroundColor(newConfig.bgColor);
  globalConfig = newConfig;
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
    leds[i].getColor(colorOut);

    //memcpy(ringChannel->values+(i*3), &colorOut, sizeof(RGBColor));

    ringChannel->values[(i*3)] = min(colorOut.r, (uint8_t) 255) / 255.0f * MULTIPLEXER_COUNTER_MAX;
    ringChannel->values[(i*3)+1] = min(colorOut.g, (uint8_t) 255) / 255.0f * MULTIPLEXER_COUNTER_MAX;
    ringChannel->values[(i*3)+2] = min(colorOut.b, (uint8_t) 255) / 255.0f * MULTIPLEXER_COUNTER_MAX;
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
  //updateDigits(profile->nixies);
  updateRingColors(profile->ledRing);
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

  globalSetConfig(savedConfig);
  file.close();
}

void mainLoop(void* data)
{
  while(1)
  {
    if( canUpdateGlobalTime){
      globalDatetime = rtc.now();
      canUpdateGlobalTime = false;
    }

    backButton.update();
    if(backButton.wasPressed())
      mainModule.back();

    selectButton.update();
    if(selectButton.wasPressed())
      mainModule.select();

    int dir = encoder.direction();
    if(dir != 0)
      mainModule.change(dir);

    applyProfile(mainModule.getProfile());
    vTaskDelay(pdMS_TO_TICKS(4));
  }
}

void setup() 
{
  Serial.begin(115200);
  initPins ();

  //Pixels
  //pixels.begin();

  //Digits
  // digitsRegister = new SPIClass(VSPI);
  // digitsRegister->begin(DIGITS_REGISTER_CLK, -1, DIGITS_REGISTER_DATA, DIGITS_REGISTER_CS);
  // digitalWrite(DIGITS_REGISTER_CS, HIGH);

  //PWM
  // for (size_t i = 0; i < NIXIE_COUNT; i++)
  // {
  //   ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
  //   ledcAttachPin(NIXIE_PINS[i], i);
  // }  

  //RTC
  // TwoWire* twoWire = new TwoWire(1);
  // twoWire->begin(26,25,100000);
  //if(!rtc.begin(twoWire)) {}

  //WiFi
  wifiConnect();
  //setTimeByNTP();

  //Interupt
  // rtc.disable32K();
  // rtc.writeSqwPinMode(Ds3231SqwPinMode::DS3231_SquareWave1Hz);
  // attachInterrupt(digitalPinToInterrupt(RTC_SQW_PIN), updateGlobalDatetime, FALLING );

  //File system
  SPIFFS.begin();

  //Web page  
  startServer();

  ringChannel = multiplexChannelFactory(LED_RING_COUNT, 3);
  ringChannels[0] = ringChannel;
  multiplexer_init(ringChannels, 1, 2);

  applyProfile(mainModule.getProfile());

  //Loop
  xTaskCreatePinnedToCore(
      mainLoop,  
      "MainLoop",  
      1024 * 2,           
      NULL,            
      1,               
      &loop_task_handle_test,
      0
  );
}

void loop()
{
  vTaskDelay(1000000);
} 