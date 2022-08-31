#pragma once

#define PWM_RESOLUTION 12
#define PWM_FREQUENCY 1000
#define PWM_MAX_VALUE (1 << PWM_RESOLUTION)

#define NEO_PIXEL_COUNT 6
#define NEO_PIXELS_PIN 32

#define DIGITS_REGISTER_CS 5
#define DIGITS_REGISTER_CLK 18
#define DIGITS_REGISTER_DATA 23
#define SPI_CLK_1MHZ 1000000

//Shift Register Multiplexer
#define MULTIPLEXER_DATA 13
#define MULTIPLEXER_CLK  14
#define MULTIPLEXER_CS   15
#define MULTIPLEXER_COUNTER_MAX 128

#define OCILATING_PERIOD 1/2
#define LED_RING_COUNT 10
#define NIXIE_COUNT 6
#define RTC_SQW_PIN 35

#define ENCODER_CLK 27
#define ENCODER_DT 36
#define ENCODER_SW 34

#define WiFi_SSID "AUREO"
#define WiFi_PASSWORD "BananaCanela"

#define CONFIG_FILE_PATH "/config.json"
//WiFi
// const char *MY_WiFi_SSID = "AUREO";
// const char *MY_WiFi_PASSWORD = "BananaCanela";
const int NIXIE_PINS[NIXIE_COUNT] = { 21, 19, 16, 17, 4, 2};