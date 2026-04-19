/**********************************************************************

   TriMix - Oxygen and helium mixer controller
   https://hilitec.net - info@hilitec.net

   @file  TriMix.h
   @brief Main header file

 **********************************************************************/
#ifndef TRIMIX_H
#define TRIMIX_H

  // Define external fonts
  // https://github.com/Bodmer/TFT_ILI9341_ESP/blob/master/examples/All_Free_Fonts_Demo/Free_Fonts.h
  #define GFXFF 1
  #define  FF1 &FreeMono9pt7b
  #define  FF2 &FreeMono12pt7b
  #define  FF3 &FreeMono18pt7b
  #define  FF4 &FreeMono24pt7b
  #define  FF7 &FreeMonoBold18pt7b
  #define  FF8 &FreeMonoBold24pt7b
  #define FF17 &FreeSans9pt7b
  #define FF21 &FreeSansBold9pt7b
  #define FF22 &FreeSansBold12pt7b
  #define FF23 &FreeSansBold18pt7b
  #define FF24 &FreeSansBold24pt7b
  #define FF29 &FreeSansOblique9pt7b
  #define FF30 &FreeSansOblique12pt7b
  #define FF31 &FreeSansOblique18pt7b
  #define FF32 &FreeSansOblique24pt7b

  // TFT instance
  TFT_eSPI tft = TFT_eSPI();

  // Sprite object
  // TFT_eSprite spr = TFT_eSprite(&tft);

  // PNG decoder instance
  PNG png;

  // Analog to digital converter object
  Adafruit_ADS1115 ads;

  // Rotary encoder object
  AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

  const int httpPort = 80;  // Assign port 80 for WebServer
  const int wsPort = 3000;  // Assign port 3000 for WebSocket

  // O2 proportion in the air
  const float fixed_ambiant_air_O2 = 20.95;

  float percent_hysteresis_he_high = 1.5;
  float percent_hysteresis_he_low = 0.5;

  float percent_hysteresis_o2_high = 0.5;
  float percent_hysteresis_o2_low = -0.5;

  // JSON object for messages to be applied to and from the client. It is
  // managed by ArduinoJson.
  StaticJsonDocument<JSON_MAX_SIZE> json_out;

  bool debug_mode = false;
  bool last_sent_debug_mode = false;

  bool spiffs_mounted = false;

  boolean app_started = false;
  boolean ads_available = false;

  int16_t steps_for_D1 = 0;
  int16_t steps_for_D2 = 0;

  float voltage_detector_for_D1 = 0.0;
  float voltage_detector_for_D2 = 0.0;

  float voltage_ambiant_air_D1 = 0.0;
  float voltage_ambiant_air_D2 = 0.0;

  String last_sent_voltage_ambiant_air_D1 = "";
  String last_sent_voltage_ambiant_air_D2 = "";

  float percent_D1 = 0.0;
  float percent_O2 = 0.0;
  float percent_HE = 0.0;

  float o2mix = fixed_ambiant_air_O2;
  float hemix = 0.0;

  uint8_t option = 0;
  uint8_t last_option = 4;
  uint8_t last_selected_option = 4;

  uint8_t ws_option = 0;

  char payload[JSON_MAX_SIZE];
  char last_payload[JSON_MAX_SIZE];

  int padding;
  int font_height;
  int font_height2;
  int font_width;
  int font_width2;

  char tmp[12];

  uint16_t value = 0;

  uint16_t running512 = 0;

  uint16_t next_y_pos = 0;

  unsigned long last_display_update = 0;
  unsigned long last_probe_update = 0;
  unsigned long last_calibration_info_update = 0;

  unsigned long start_calibration = 0;

  unsigned long start_fill = 0;

  unsigned long last_time_update = 0;

  boolean display_update = true;
  boolean calibration_info_update = true;

  boolean select_option = false;
  boolean ws_select_option = false;
  boolean last_select_option = false;
  
  boolean ws_option_flag = false;

  boolean compressor_state_required = false;
  int compressor_state_must_is = 100; // (100 = off/off = black, 111 = on/on = green, 101 = off/on = red, 110 = on/off = yellow)
  int last_compressor_state_must_is = 0;
  unsigned long  compressor_last_change_time = 0;

  boolean he_valve_is_on = false;
  boolean o2_valve_is_on = false;

  boolean last_he_valve_state = !he_valve_is_on;
  boolean last_o2_valve_state = !o2_valve_is_on;

  boolean compressor_set_value = false;
  unsigned long compressor_set_time = 0;

  unsigned long now = 0;

  unsigned long last_he_valve_changed_state = 0;
  unsigned long last_o2_valve_changed_state = 0;

  unsigned long total_fill_time = 0;

  unsigned long total_he_valve_on = 0;
  unsigned long total_he_valve_off = 0;

  unsigned long running_he_valve_on = 0;
  unsigned long running_he_valve_off = 0;

  unsigned long total_o2_valve_on = 0;
  unsigned long total_o2_valve_off = 0;

  unsigned long running_o2_valve_on = 0;
  unsigned long running_o2_valve_off = 0;

  uint16_t color_o2 = COLOR565_O2; // WHITE
  uint16_t color_he = COLOR565_HE; // BROWN
  uint16_t color_calibration = COLOR565_CALIBRATION; // BLUE
  uint16_t color_fill = COLOR565_FILL; // RED

  uint16_t color_o2_dark = COLOR565_O2_DARK; // DARK GREY
  uint16_t color_he_dark = COLOR565_HE_DARK; // DARK BROWN
  uint16_t color_calibration_dark = COLOR565_CALIBRATION_DARK; // DARK BLUE
  uint16_t color_fill_dark = COLOR565_FILL_DARK; // DARK RED

  uint16_t color_calibration_light = COLOR565_CALIBRATION_LIGHT; // LIGHT BLUE
  uint16_t color_fill_light = COLOR565_FILL_LIGHT; // LIGHT RED

  uint16_t color_calibration_flashing = color_calibration_light;
  uint16_t last_color_calibration_flashing = color_calibration_flashing;

  uint16_t color_fill_flashing = color_fill_light;
  uint16_t last_color_fill_flashing = color_fill_flashing;

  uint16_t color_o2_current = color_o2;
  uint16_t color_he_current = color_he;
  uint16_t color_calibration_current = color_calibration;
  uint16_t color_fill_current = color_fill;

  uint16_t color_o2_back = color_o2;
  uint16_t color_he_back = color_he;
  uint16_t color_calibration_back = color_calibration;
  uint16_t color_fill_back = color_fill;

  uint16_t color_running = COLOR565_RUNNING_DARK;
  uint16_t last_color_running = 99;

  time_t last_calibration_time_epoch = 0;
  String last_calibration_time = "";
  String last_sent_calibration_time = "";

  // Display layout
  int16_t margin_left   = MARGIN_WIDTH;
  int16_t margin_right  = MARGIN_WIDTH;
  int16_t margin_top    = MARGIN_TOP;
  int16_t margin_bottom = MARGIN_BOTTOM;

  int16_t padding_box   = PADDING_BOX;

  int16_t upper_box_left_top     = margin_top;
  int16_t upper_box_left_left    = margin_left;
  int16_t upper_box_left_center  = margin_left + (BOX_WIDTH/2);
  int16_t upper_box_left_middle  = margin_top + (BOX_HEIGHT/2);
  int16_t upper_box_right_top    = margin_top;
  int16_t upper_box_right_left   = SCREEN_WIDTH - margin_right - BOX_WIDTH;
  int16_t upper_box_right_center = SCREEN_WIDTH - margin_right - (BOX_WIDTH/2);
  int16_t upper_box_right_middle = margin_top + (BOX_HEIGHT/2);

  int16_t between_box_center     = margin_top + BOX_HEIGHT + ((SCREEN_HEIGHT - margin_top - margin_bottom - (2 * BOX_HEIGHT))/2);

  int16_t lower_box_left_top     = SCREEN_HEIGHT - margin_bottom - BOX_HEIGHT;
  int16_t lower_box_left_left    = margin_left;
  int16_t lower_box_left_center  = margin_left + (BOX_WIDTH/2);
  int16_t lower_box_left_middle  = SCREEN_HEIGHT - margin_bottom - (BOX_HEIGHT/2);
  int16_t lower_box_right_top    = SCREEN_HEIGHT - margin_bottom - BOX_HEIGHT;
  int16_t lower_box_right_left   = SCREEN_WIDTH - margin_right - BOX_WIDTH;
  int16_t lower_box_right_center = SCREEN_WIDTH - margin_right - (BOX_WIDTH/2);
  int16_t lower_box_right_middle = SCREEN_HEIGHT - margin_bottom - (BOX_HEIGHT/2);

  int16_t selected_x[5] = {upper_box_left_left, upper_box_right_left, lower_box_left_left, lower_box_right_left, upper_box_left_left};
  int16_t selected_y[5] = {upper_box_left_top, upper_box_right_top, lower_box_left_top, lower_box_right_top, upper_box_left_top};

  uint16_t selected_color[5] = {color_o2, color_he, color_calibration, color_fill, color_o2};

#endif