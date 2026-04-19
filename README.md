# TriMix
TriMix is an ESP32 based mixer controller for Oxygen (O2) and Helium (He), with TFT display, Wifi connection and Web interface

# Hardware
Microcontroller: ESP32
Board: ESP32 board with 4 integrated relays
Button: Rotary Digital Encoder Push Button (like EC11) for the setup
Analog converter: I2C ADS1115 4channels analog to digital converter
Oxygen sensor: NRC D-05
Display: SPI 3.5" TFT display with ST7796S controller

 # WARNING !
 PIN 12 must NEVER be at Vcc during boot sequence
 ADC2 pins cannot be used when Wi-Fi is used
 
 # Sponsoring
 This package is the result of a *LOT* of work. If you are happy using this package, contact us for a donation to support this project.
 