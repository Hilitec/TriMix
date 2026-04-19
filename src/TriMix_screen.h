#ifndef TRIMIX_SCREEN_H
#define TRIMIX_SCREEN_H

  #define USER_SETUP_LOADED
  #define ST7796_DRIVER
  #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red
  #define TFT_WIDTH  320
  #define TFT_HEIGHT 480
  #define TFT_INVERSION_OFF

  // #define TFT_MISO -1
  #define TFT_MOSI 13 // 23
  #define TFT_SCLK 14 // 18
  #define TFT_CS   15  // Chip select control pin
  #define TFT_DC    2  // Data Command control pin
  #define TFT_RST   4  // Reset pin (could connect to RST pin)
  // #define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

  #define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
  #define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
  #define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
  #define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
  // #define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
  // #define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
  //#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
  // #define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

  // Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
  // this will save ~20kbytes of FLASH
  #define SMOOTH_FONT

  // #define SPI_FREQUENCY   1000000
  // #define SPI_FREQUENCY   5000000
  // #define SPI_FREQUENCY  10000000
  // #define SPI_FREQUENCY  20000000
  #define SPI_FREQUENCY  27000000 // This was working with ESP32/ST7796
  // #define SPI_FREQUENCY  40000000
  // #define SPI_FREQUENCY  55000000 // STM32 SPI1 only (SPI2 maximum is 27MHz)
  // #define SPI_FREQUENCY  80000000

  // Optional reduced SPI frequency for reading TFT
  #define SPI_READ_FREQUENCY  20000000

  // The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
  #define SPI_TOUCH_FREQUENCY  2500000

#endif