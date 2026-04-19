/**********************************************************************

   TriMix - Oxygen and helium mixer controller
   https://hilitec.net - info@hilitec.net

   @file  TriMix_includes.h
   @brief Include all external libraries

 **********************************************************************/
#ifndef TRIMIX_INCLUDES_H
#define TRIMIX_INCLUDES_H

  #include <Arduino.h>

  #include <WiFi.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>

  // https://github.com/dvarrel/AsyncTCP
  #include <AsyncTCP.h>

  // https://github.com/dvarrel/ESPAsyncWebSrv
  #include <ESPAsyncWebSrv.h>

  // Wire for ADS1115
  #include <Wire.h>

  // SPI for TFT_eSPI
  #include <SPI.h>

  #include <SPIFFS.h>

  // https://github.com/Hieromon/AutoConnect/
  // https://hieromon.github.io/AutoConnect/
  // #include <AutoConnectCore.h>
  #include <AutoConnect.h>

  #include <ArduinoJson.h>
  #if ARDUINOJSON_VERSION_MAJOR < 6
    #error "Requires ArduinoJson version 6 or later"
  #endif

  // https://microcontrollerslab.com/ads1115-external-adc-with-esp32/
  #include <Adafruit_ADS1X15.h>

  // https://github.com/igorantolic/ai-esp32-rotary-encoder
  #include "AiEsp32RotaryEncoder.h"

  // https://github.com/Bodmer/TFT_eSPI
  #include <TFT_eSPI.h> // Hardware-specific library
  #undef ILI9341_DRIVER // Generic driver for common displays

  // https://github.com/Bodmer/TFT_eWidget
  #include <TFT_eWidget.h>

  // TFT alternative
  // https://github.com/moononournation/Arduino_GFX
  // https://github.com/moononournation/ArduinoFreeFontFile

  // https://github.com/bitbank2/PNGdec
  #include <PNGdec.h>

#endif