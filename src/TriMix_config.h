/**********************************************************************

   TriMix - Oxygen and helium mixer controller
   https://hilitec.net - info@hilitec.net

   @file  TriMix_config.h
   @brief Configuration file

 **********************************************************************/
#ifndef TRIMIX_CONFIG_H
#define TRIMIX_CONFIG_H

  // 1,2,4,6,7,8
  #define FONT_SIZE_INFO_CALIBRATION 2
  #define FONT_SIZE_PROBE_MV         2
  #define FONT_SIZE_TOTAL_TIME       2
  #define FONT_SIZE_VALVE_PERCENT    2

  #define JSON_MAX_SIZE 512

  // Converter : http://www.rinkydinkelectronics.com/calc_rgb565.php
  // TFT colors (search TFT_MAGENTA): https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_eSPI.h
  #define COLOR565_BACKGROUND              0x0000 // BLACK 0,0,0
  #define COLOR565_TEXT_WELCOME            0xFFE0 // YELLOW 255,255,0
  #define COLOR565_TEXT_INFO               0xFFE0 // YELLOW 255,255,0
  #define COLOR565_SELECT_BOX              0x07E0 // GREEN 0,255,0
  #define COLOR565_O2                      0xFFFF // WHITE 255,255,255
  #define COLOR565_HE                      0x9A60 // BROWN 150,75,0 - 0x82A4 // BROWN 133,87,35
  #define COLOR565_CALIBRATION             0x39FF // BLUE 63,63,255 (instead of 0x001F - 0,0,255)
  #define COLOR565_FILL                    0xF800 // RED 255,0,0
  #define COLOR565_CALIBRATION_DARK        0x0003 // DARK BLUE 0,0,31
  #define COLOR565_FILL_DARK               0x1800 // DARK RED 31,0,0
  #define COLOR565_O2_DARK                 0x18E3 // DARK GREY 31,31,31
  #define COLOR565_HE_DARK                 0x2080 // DARK BROWN 37,19,0 - 0x20A1 // DARK BROWN 33,22,9
  #define COLOR565_CALIBRATION_LIGHT       0xBDFF // LIGHT BLUE 191,191,255
  #define COLOR565_FILL_LIGHT              0xFDF7 // DARK RED 255,191,191
  #define COLOR565_RUNNING_DARK            0x03EF // DARK CYAN 0,128,128
  #define COLOR565_RUNNING_LIGHT           0x07FF // CYAN 0,255,255
  #define COLOR565_DEBUG                   0xFDF7 // PINK 255,191,191
  #define COLOR565_ALERT                   0xF800 // RED 255,0,0
  #define COLOR565_COMPRESSOR_ON           0x07E0 // GREEN 0,255,0
  #define COLOR565_COMPRESSOR_OFF          0x0000 // BLACK 0,0,0
  #define COLOR565_COMPRESSOR_ROUND        0x18E3 // DARK GREY 31,31,31
  #define COLOR565_COMPRESSOR_SHOULD_START 0xFFE0 // YELLOW 255,255,0
  #define COLOR565_COMPRESSOR_SHOULD_STOP  0xF800 // RED 255,0,0
  #define COLOR565_VALVE_OFF               0x0000 // BLACK 0,0,0
  
  #define RUNNING_CIRCLE_RADIUS      10

  #define COMPRESSOR_PULSE_TYPE       1
  #define MAX_COMPRESSOR_SET_TIME  2000
  #define COMPRESSOR_PULSE_TIME     200

  #define COMPRESSOR_PULSE_TIME     200
  
  #define MIN_STARTUP_SPLASH_TIME  5000

  #define MDNS_TIMEOUT           120000

  #define BOX_WIDTH     210
  #define BOX_HEIGHT    120
  #define MARGIN_WIDTH  10
  #define MARGIN_HEIGHT 10
  #define MARGIN_TOP    10
  #define MARGIN_BOTTOM 30
  #define PADDING_BOX   10

  #define SCREEN_WIDTH  480
  #define SCREEN_HEIGHT 320

  #define SCREEN_BORDER_HEIGHT 2
  #define SCREEN_BORDER_WIDTH 10

  #define UPDATE_REFRESH            1000
  #define MAX_PROBE_REFRESH          200
  #define CALIBRATION_INFO_REFRESH 10000

  #define MIN_CALIBRATION_TIME 20000
  #define MAX_CALIBRATION_TIME 30000

  #define AP_SSID "TriMixSetup"
  #define AP_PSK ""

  #define GMT_OFFSET 3600
  #define DAYLIGHT_OFFSET 3600
  #define NTP_SERVER "ntp.metas.ch"

  #define O2_PERCENT_ALERT        40.0

  #define RELAY_COMPRESSOR          32 // Relay 1, Compressor
  #define RELAY_HE_VALVE            33 // Relay 2, He solenoid valve
  #define RELAY_O2_VALVE            25 // Relay 3, O2 solenoid valve
  #define RELAY_4                   26 // Relay 4
  
  #define BUILTIN_LED               23
  #define LED_BUILTIN               23

  #define ROTARY_ENCODER_BUTTON_PIN 12 // Digital input, rotary encoder button make a contact with GND !!! The PIN12 must NEVER be at Vcc during boot !!!
  #define ROTARY_ENCODER_A_PIN       5 // Digital input, rotary encoder center PIN is connected on Vcc
  #define ROTARY_ENCODER_B_PIN      19 // Digital input, rotary encoder center PIN is connected on Vcc
  #define ROTARY_ENCODER_STEPS       8
  #define ROTARY_ENCODER_VCC_PIN    -1 // We don't define a Vcc pin for the rotary encoder

  #define SDA_PIN                   21
  #define SCL_PIN                   22
  
  #define COMPRESSOR_STATE_PIN      18 // This is compressor on detection, make a contact with GND

  #define PNG_MAX_IMAGE_WIDTH      480

  #define STATE_OFF              "OFF"
  #define STATE_ON                "ON"

  #define DEBUG_D1_VALUE           6.0
  #define DEBUG_D2_VALUE          12.0

  #define HOSTIDENTIFY  "trimix"
  #define mDNSUpdate(c)  do {} while(0)

  const String spiffs_detector_for_D1_ambiant_air = "/detectD1.ini";
  const String spiffs_detector_for_D2_ambiant_air = "/detectD2.ini";
  const String spiffs_calibration_time = "/caltime.ini";
  const String spiffs_last_hemix_value = "/hemix.ini";
  const String spiffs_last_o2mix_value = "/o2mix.ini";

  /**/
  #define DEBUG_PRINT(x)               Serial.print   (x)
  #define DEBUG_PRINTF(x,y)            Serial.printf  (x, y)
  #define DEBUG_PRINTFF(x,y,z)         Serial.printf  (x, y, z)
  #define DEBUG_PRINTFFF(w,x,y,z)      Serial.printf  (w, x, y, z)
  #define DEBUG_PRINTFFFF(v,w,x,y,z)   Serial.printf  (v, w, x, y, z)
  #define DEBUG_PRINTDEC(x)            Serial.print   (x, DEC)
  #define DEBUG_PRINTHEX(x)            Serial.print   (x, HEX)
  #define DEBUG_PRINTLN(x)             Serial.println (x)
  #define DEBUG_INFO(str)              \
    Serial.print(millis());            \
    Serial.print(": ");                \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.print(' ');                 \
    Serial.print(__FILE__);            \
    Serial.print(':');                 \
    Serial.print(__LINE__);            \
    Serial.print(' ');                 \
    Serial.println(str);
/**/
/*
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTF(x,y)
  #define DEBUG_PRINTFF(x,y,z)
  #define DEBUG_PRINTFFF(w,x,y,z)
  #define DEBUG_PRINTFFFF(v,w,x,y,z)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTHEX(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_INFO(str)
*/

#endif