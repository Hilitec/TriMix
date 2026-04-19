/**********************************************************************
 * 
 * TriMix - Oxygen and helium mixer controller
 * https://hilitec.net - info@hilitec.net
 * (c) 2023 Hilitec.net
 * 
 * @file  TriMix.ino
 * @brief Main file of the TriMix package
 * 
 * TriMix is a mixer controller for Oxygen (O2) and Helium (He).
 * 
 * The TriMix concept is based on the following elements:
 * - ESP32 board with 4 integrated relays
 * - Rotary Digital Encoder Push Button (like EC11) for the setup
 * - I2C ADS1115 4channels analog to digital converter
 * - SPI 3.5" TFT display with ST7796S controller
 * 
 * This package is the result of a *LOT* of work. If you are happy using this
 * package, contact us for a donation to support this project.
 * 
 * !!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!
 * ! PIN 12 must NEVER be at Vcc during boot sequence !
 * ! ADC2 pins cannot be used when Wi-Fi is used      !
 * !!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!
 * 
 * @author    Andre Liechti, Hilitec.net */
const String ACTUAL_FIRMWARE = "0.9.1.2";
/* @date      2024-03-05
 * @since     2023-11-26 */
const String APP_COPYRIGHT = "(c) 2023-2024 Hilitec.net";
const String APP_COPYRIGHT_HTML = "&copy; 2023-2024 Hilitec.net";
/* @copyright GNU Lesser General Public License
 * 
 * ESP32 Partition organization:
 * - Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)
 * 
 * Please check TriMix_config.h for option definitions
 * 
 * Changes log:

 *   See CHANGELOG.md
 * 
 **********************************************************************/
const String APPLICATION_NAME = "TriMix";
const char COMPILE_DATE[] = __DATE__ " " __TIME__;

#include "TriMix_config.h"
#include "TriMix_includes.h"
#include "TriMix_logo.h"
#include "TriMix_english.h"
#include "TriMix.h"

// The server instance for WebSocket uses ESPAsyncWebServer.
AsyncWebServer wsServer(wsPort);
AsyncWebSocket ws("/");

static const char* hostname = HOSTIDENTIFY;

WebServer server(httpPort);

AutoConnect portal(server);
AutoConnectConfig config;

void sendRedirect(String);

// Define page_relays
#include "TriMix_page_relays.h"

static const char PROGMEM autoconnectMenu[] = { AUTOCONNECT_LINK(BAR_24) };

void SetCompState(boolean value) {
  if (value != compressor_state_required) {
    compressor_last_change_time = millis();
    compressor_state_required = value;
  }
  CheckAndUpdateCompressor();
}

void CheckAndUpdateCompressor() {
  boolean compressor_state = !digitalRead(COMPRESSOR_STATE_PIN);
  if ((compressor_state != compressor_state_required) && (now > (compressor_last_change_time + MAX_COMPRESSOR_SET_TIME))) {
    compressor_last_change_time = now;
  }
  if ((now < (compressor_last_change_time + COMPRESSOR_PULSE_TIME)) && (compressor_last_change_time > 0) && (compressor_state != compressor_state_required)) {
    digitalWrite(RELAY_COMPRESSOR, HIGH);
  } else {
    digitalWrite(RELAY_COMPRESSOR, LOW);
  }
  compressor_state_must_is = (100 + (compressor_state_required ? 10 : 0) + (compressor_state ? 1 : 0));
}

String readStateRelayCompressor(PageArgument& args) {
  return digitalRead(RELAY_COMPRESSOR) ? STATE_ON : STATE_OFF;
}

String readStateRelayValveHe(PageArgument& args) {
  return digitalRead(RELAY_HE_VALVE) ? STATE_ON : STATE_OFF;
}

String readStateRelayValveO2(PageArgument& args) {
  return digitalRead(RELAY_O2_VALVE) ? STATE_ON : STATE_OFF;
}

String readApplicationInfo(PageArgument& args) {
  return String(APPLICATION_NAME) + " " + String(ACTUAL_FIRMWARE);
}

String readIpAddress(PageArgument& args) {
  return WiFi.localIP().toString();
}

String readAppCopyright(PageArgument& args) {
  return String(APP_COPYRIGHT_HTML);
}

String readO2Mix(PageArgument& args) {
  return String(dtostrf(o2mix,3,1,tmp));
}

String readHeMix(PageArgument& args) {
  return String(dtostrf(hemix,3,1,tmp));
}

String readCalibratedDate(PageArgument& args) {
  return last_calibration_time;
}

String readCalibratedD1(PageArgument& args) {
  return String(dtostrf(voltage_ambiant_air_D1,3,2,tmp));
}

String readCalibratedD2(PageArgument& args) {
  return String(dtostrf(voltage_ambiant_air_D2,3,2,tmp));
}

PageElement elm_gpio(page_relays, { { "IP_ADDRESS",       readIpAddress },
                                    { "O2_MIX",           readO2Mix },
                                    { "HE_MIX",           readHeMix },
                                    { "CALIBRATED_DATE",  readCalibratedDate },
                                    { "CALIBRATED_D1",    readCalibratedD1 },
                                    { "CALIBRATED_D2",    readCalibratedD2 },
                                    { "APP_COPYRIGHT",    readAppCopyright },
                                    { "APPLICATION_INFO", readApplicationInfo },
                                    { "AUTOCONNECT_MENU", [](PageArgument& args) {
                                       return String(FPSTR(autoconnectMenu));
                                     }
                                    }
                                  });
PageBuilder root("/", { elm_gpio });


String gpio(PageArgument& args) {
  if (args.arg("v") == "r1off")
    digitalWrite(RELAY_COMPRESSOR, LOW);
  else if (args.arg("v") == "r1on")
    digitalWrite(RELAY_COMPRESSOR, HIGH);
  else if (args.arg("v") == "r2off")
    digitalWrite(RELAY_HE_VALVE, LOW);
  else if (args.arg("v") == "r2on")
    digitalWrite(RELAY_HE_VALVE, HIGH);
  else if (args.arg("v") == "r3off")
    digitalWrite(RELAY_O2_VALVE, LOW);
  else if (args.arg("v") == "r3on")
    digitalWrite(RELAY_O2_VALVE, HIGH);
  sendRedirect("/");
  return "";
}

PageElement elm_io("{{IO}}", { { "IO", gpio } });
PageBuilder io("/io", { elm_io });

// This function is for redirect only.
// The actual sending the HTML performs in PageBuilder.
void sendRedirect(String uri) {
  server.sendHeader("Location", uri, true);
  server.send(302, "text/plain", "");
  server.client().stop();
  io.cancel();
}

bool atDetect(IPAddress& softapIP) {
 DEBUG_PRINTLN("Captive portal started, SoftAP IP:" + softapIP.toString());
  return true;
}

void exitOTAStart() {
 DEBUG_PRINTLN("OTA started");
}

void exitOTAProgress(unsigned int amount, unsigned int sz) {
 // DEBUG_PRINTFF("OTA in progress: received %d bytes, total %d bytes\n", sz, amount);
}

void exitOTAEnd() {
 DEBUG_PRINTLN("OTA ended");
}

void exitOTAError(uint8_t err) {
 DEBUG_PRINTF("OTA error occurred %d\n", err);
}

void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

// https://github.com/Hieromon/AutoConnect/blob/master/examples/WebSocketServer/WebSocketServer.ino
// The following event handlers for AsyncWebSocket correspond to message
// reception events.
void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {

  StaticJsonDocument<JSON_MAX_SIZE> json_in;

  DEBUG_PRINTFF("[WS] Event(%u):%d\n", client->id(), type);

  // It applies the screening process to extract the message body with the event
  // type = WS_EVT_DATA.
  if (type == WS_EVT_DATA) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len) {
     DEBUG_PRINTFF("[WS] (%u):%s ", client->id(), server->url());
      if (info->opcode == WS_TEXT) {
        data[len] = '\0';
       DEBUG_PRINTLN((const char*)data);
        DeserializationError error = deserializeJson(json_in, (const char*)data);
        JsonObject root = json_in.as<JsonObject>();

        if (error) {
         DEBUG_PRINT("deserializeJson() failed: ");
         DEBUG_PRINTLN(error.c_str());
        } else {
          // Do something with the data !
          // updatePeriod((const char*)data);

          // tft.setFreeFont(FF17); // Because % is not existing in font size 6, only numbers (like 7 and 8)
          // tft.setTextDatum(TL_DATUM);
          // tft.setTextColor(COLOR565_TEXT_INFO, color_fill_back);
          for (JsonPair kv : root) {
            //if (String("rhe").equals(kv.key().c_str())) {
            //  tft.drawString("He: " + String(kv.value().as<const char*>()), lower_box_right_left+2, lower_box_right_top+20, GFXFF);
            //} else if (String("ro2").equals(kv.key().c_str())) {
            //  tft.drawString("O2: " + String(kv.value().as<const char*>()), lower_box_right_left+2, lower_box_right_top+20, GFXFF);
            //}
            if (String("o2mix").equals(kv.key().c_str())) {
              o2mix = String(kv.value().as<const char*>()).toFloat();
              if (spiffs_mounted) {
                File fileO2Mix = SPIFFS.open(spiffs_last_o2mix_value, FILE_WRITE);
                fileO2Mix.print(String(o2mix));
                fileO2Mix.close();
              }
            } else if (String("hemix").equals(kv.key().c_str())) {
              hemix = String(kv.value().as<const char*>()).toFloat();
              if (spiffs_mounted) {
                File fileHeMix = SPIFFS.open(spiffs_last_hemix_value, FILE_WRITE);
                fileHeMix.print(String(hemix));
                fileHeMix.close();
              }
            } else if (String("option").equals(kv.key().c_str())) {
              ws_option = String(kv.value().as<const char*>()).toInt();
              ws_option_flag = true;
            } else if (String("selectOption").equals(kv.key().c_str())) {
              ws_select_option = (1 == String(kv.value().as<const char*>()).toInt());
              ws_option_flag = true;
            } else if (String("debugMode").equals(kv.key().c_str())) {
              debug_mode = (1 == String(kv.value().as<const char*>()).toInt());
              app_started = false;
            }
          }
        }
      } else {
        for (size_t i = 0; i < info->len; i++) {
         DEBUG_PRINTF("%02x ", data[i]);
        }
       DEBUG_PRINTLN();
      }
    }
  }
}

void setup() {

  Serial.begin(115200);
  DEBUG_PRINTLN();

  tft.init();
  tft.setRotation(1);

  tft.fillScreen(COLOR565_BACKGROUND);

  pinMode(RELAY_COMPRESSOR, OUTPUT);
  pinMode(RELAY_HE_VALVE, OUTPUT);
  pinMode(RELAY_O2_VALVE, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  digitalWrite(RELAY_COMPRESSOR, LOW);
  digitalWrite(RELAY_HE_VALVE, LOW);
  digitalWrite(RELAY_O2_VALVE, LOW);
  digitalWrite(RELAY_4, LOW);

  pinMode(COMPRESSOR_STATE_PIN, INPUT_PULLUP);
  pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT_PULLUP);

  // DEBUG MODE, based on the ROTARY_ENCODER_BUTTON_PIN status on boot
  debug_mode = !digitalRead(ROTARY_ENCODER_BUTTON_PIN);

  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setAcceleration(250);
  rotaryEncoder.setBoundaries(0, 3, false);  // 0-3 with loop
  rotaryEncoder.setEncoderValue(0);

  int16_t rc = png.openFLASH((uint8_t *)TriMix_logo, sizeof(TriMix_logo), pngDraw);
  if (rc == PNG_SUCCESS) {
   DEBUG_PRINTLN("Successfully opened png file");
   DEBUG_PRINTFFFF("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    DEBUG_PRINT(millis() - dt);DEBUG_PRINTLN("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }

  tft.setTextColor(COLOR565_TEXT_WELCOME); // No background colour will keep unmodified things under the text

  // Display copyright info
  tft.setFreeFont(FF21);
  tft.setTextDatum(BR_DATUM);
  tft.drawString(String(APP_COPYRIGHT), tft.width()-SCREEN_BORDER_WIDTH, tft.height()-SCREEN_BORDER_HEIGHT, GFXFF);

  // Display application name
  tft.setFreeFont(FF24);
  font_height = tft.fontHeight(GFXFF);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(String(APPLICATION_NAME) + " " + String(ACTUAL_FIRMWARE), SCREEN_BORDER_WIDTH, SCREEN_BORDER_HEIGHT, GFXFF);
  next_y_pos = SCREEN_BORDER_HEIGHT + font_height;

  // Display compilation date and time
  font_height = tft.fontHeight(2);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(String(COMPILE_DATE), SCREEN_BORDER_WIDTH, next_y_pos, 2);
  next_y_pos = next_y_pos + font_height;

  // Display initialization subtitle
  tft.setFreeFont(FF21);
  font_height = tft.fontHeight(GFXFF);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(String(TXT_INITIALIZING), SCREEN_BORDER_WIDTH, next_y_pos + 4, GFXFF);
  next_y_pos = next_y_pos + 4 + font_height;

  if (esp_spiffs_mounted(NULL)) {
    spiffs_mounted = true;
  } else {
    spiffs_mounted = SPIFFS.begin(true);
  }

  if (!spiffs_mounted) {
    // Display SPIFFS error
    tft.setFreeFont(FF21);
    font_height = tft.fontHeight(GFXFF);
    tft.setTextDatum(TL_DATUM);
    tft.drawString(String(ERR_SPIFFS_NOT_MOUNTED), SCREEN_BORDER_WIDTH, next_y_pos, GFXFF);
    next_y_pos = next_y_pos + font_height;
  } else {

    if (SPIFFS.exists(spiffs_detector_for_D1_ambiant_air)) {
      File fileHE = SPIFFS.open(spiffs_detector_for_D1_ambiant_air);
      voltage_ambiant_air_D1 = fileHE.readString().toFloat();
      fileHE.close();
     DEBUG_PRINTLN("D1 probe ambiant air value (near HE): " + String(voltage_ambiant_air_D1));
    } else {
      voltage_ambiant_air_D1 = 10.0;
      File fileHE = SPIFFS.open(spiffs_detector_for_D1_ambiant_air, FILE_WRITE);
      fileHE.print(String(voltage_ambiant_air_D1));
      fileHE.close();
     DEBUG_PRINTLN("HE probe ambiant air value initialized");
    }
    if (voltage_ambiant_air_D1 < 1.0) {
      voltage_ambiant_air_D1 = 1.0;
    }

    if (SPIFFS.exists(spiffs_detector_for_D2_ambiant_air)) {
      File fileO2 = SPIFFS.open(spiffs_detector_for_D2_ambiant_air);
      voltage_ambiant_air_D2 = fileO2.readString().toFloat();
      fileO2.close();
     DEBUG_PRINTLN("D2 probe ambiant air value (near O2): " + String(voltage_ambiant_air_D2));
    } else {
      voltage_ambiant_air_D2 = 10.0;
      File fileO2 = SPIFFS.open(spiffs_detector_for_D2_ambiant_air, FILE_WRITE);
      fileO2.print(String(voltage_ambiant_air_D2));
      fileO2.close();
     DEBUG_PRINTLN("O2 probe ambiant air value initialized");
    }
    if (voltage_ambiant_air_D2 < 1.0) {
      voltage_ambiant_air_D2 = 1.0;
    }

    if (SPIFFS.exists(spiffs_calibration_time)) {
      File fileCT = SPIFFS.open(spiffs_calibration_time);
      last_calibration_time_epoch = atol(fileCT.readString().c_str());
      last_calibration_time = printLocalTime(TXT_FORMAT_CALIBRATION, last_calibration_time_epoch);
      fileCT.close();
      DEBUG_PRINTLN("Calibration time: " + last_calibration_time);
    } else {
      last_calibration_time_epoch = 1704063600;
      last_calibration_time = printLocalTime(TXT_FORMAT_CALIBRATION, last_calibration_time_epoch);
      File fileCT = SPIFFS.open(spiffs_calibration_time, FILE_WRITE);
      fileCT.print(String(last_calibration_time_epoch));
      fileCT.close();
      DEBUG_PRINTLN("Calibration time initialized");
    }

    if (SPIFFS.exists(spiffs_last_hemix_value)) {
      File fileHe = SPIFFS.open(spiffs_last_hemix_value);
      hemix = fileHe.readString().toFloat();
      fileHe.close();
     DEBUG_PRINTLN("He mix value: " + String(hemix));
    } else {
      hemix = 0.0;
      File fileHe = SPIFFS.open(spiffs_last_hemix_value, FILE_WRITE);
      fileHe.print(String(hemix));
      fileHe.close();
     DEBUG_PRINTLN("He mix value initialized");
    }
    if (hemix < 0.0) {
      hemix = 0.0;
    } else if (hemix > 99.9) {
      hemix = 99.9;
    }

    if (SPIFFS.exists(spiffs_last_o2mix_value)) {
      File fileO2 = SPIFFS.open(spiffs_last_o2mix_value);
      o2mix = fileO2.readString().toFloat();
      fileO2.close();
     DEBUG_PRINTLN("O2 mix value: " + String(o2mix));
    } else {
      o2mix = 0.0;
      File fileO2 = SPIFFS.open(spiffs_last_o2mix_value, FILE_WRITE);
      fileO2.print(String(o2mix));
      fileO2.close();
     DEBUG_PRINTLN("O2 mix value initialized");
    }
    if (o2mix < 0.0) {
      o2mix = 0.0;
    } else if (o2mix >= O2_PERCENT_ALERT) {
      o2mix = O2_PERCENT_ALERT - 0.1;
    }

    voltage_ambiant_air_D1 = DEBUG_D1_VALUE;
    voltage_ambiant_air_D2 = DEBUG_D2_VALUE;
  }

  // Set time and date configuration
  configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER);

  // Register an event handler for WebSocket and start ESPAsyncWebServer.
  ws.onEvent(onEvent);
  wsServer.addHandler(&ws);

  config.autoReconnect = true;  // Attempt automatic reconnection.
  config.reconnectInterval = 6;
  config.autoRise = true;         // Default is true
  config.immediateStart = false;  // Default is false
  config.retainPortal = false;    // Default is false

  config.apid = AP_SSID;
  config.psk = AP_PSK;

  config.ota = AC_OTA_BUILTIN;

  portal.config(config);
  portal.onOTAStart(exitOTAStart);
  portal.onOTAEnd(exitOTAEnd);
  portal.onOTAProgress(exitOTAProgress);
  portal.onOTAError(exitOTAError);

  // Put the home location of the web site.
  // But in usually, setting the home uri is not needed cause default location is "/".
  //portal.home("/");

  portal.onConnect([](IPAddress ip) {
    DEBUG_PRINTLN("WebSocket started for IP address: " + ip.toString());
    startMDNS();
    wsServer.begin();
  });

  // Starts user web site included the AutoConnect portal.
  portal.onDetect(atDetect);
  if (portal.begin()) {
    // Register the page request handlers.
    root.insert(server);
    io.insert(server);
    DEBUG_PRINTLN("Started, IP:" + WiFi.localIP().toString());
  }

  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  if (!ads.begin(0x48)) {
    DEBUG_PRINTLN("Failed to initialize ADS.");
  } else {
    ads_available = true;
    ads.setGain(GAIN_SIXTEEN);

    // Start the first conversion.
    //ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, /*continuous=*/false);
    //ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_2_3, /*continuous=*/false);
  }

  DEBUG_PRINTLN("End of setup().");

  while (millis() < MIN_STARTUP_SPLASH_TIME) {
    delay(100);
  }

}

void loop() {

  now = millis();

  if (!app_started) {
    DEBUG_PRINTLN("loop() started.");

    if (debug_mode) {
      tft.fillScreen(COLOR565_DEBUG);
    } else {
      tft.fillScreen(COLOR565_BACKGROUND);
    }
    tft.setTextColor(COLOR565_TEXT_INFO, COLOR565_BACKGROUND);

    tft.setFreeFont(FF17);
    tft.setTextDatum(ML_DATUM);
    tft.drawString(String(TXT_IP_ADDRESS) + ": " + WiFi.localIP().toString(), margin_left, between_box_center, GFXFF); // 2 or GFXFF

    tft.setFreeFont(FF17);
    tft.setTextDatum(BL_DATUM);
    tft.drawString(String(APPLICATION_NAME) + " " + String(ACTUAL_FIRMWARE), SCREEN_BORDER_WIDTH, tft.height()-SCREEN_BORDER_HEIGHT, GFXFF);

    tft.setFreeFont(FF17);
    tft.setTextDatum(BR_DATUM);
    tft.drawString(String(APP_COPYRIGHT), tft.width()-SCREEN_BORDER_WIDTH, tft.height()-SCREEN_BORDER_HEIGHT, GFXFF);

    if (debug_mode) {
      tft.setFreeFont(FF17);
      tft.setTextDatum(BR_DATUM);
      tft.drawString("! DEBUG !", int(SCREEN_WIDTH/2), tft.height()-SCREEN_BORDER_HEIGHT, GFXFF);
    }

  }

  portal.handleClient();
  /*
    if (WiFi.status() == WL_IDLE_STATUS) {
      ESP.restart();
      delay(1000);
    }
    */

  // Read probe state
  he_valve_is_on = digitalRead(RELAY_HE_VALVE);
  o2_valve_is_on = digitalRead(RELAY_O2_VALVE);

  CheckAndUpdateCompressor();

  if (last_compressor_state_must_is != compressor_state_must_is) {
    switch (compressor_state_must_is) {
      case 100: // must be off, is off
        tft.fillCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_OFF);
        tft.drawCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_ROUND);
        break;
      case 111: // must be on, is on
        tft.fillCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_ON);
        tft.drawCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_ROUND);
        break;
      case 110: // must be on, is off
        tft.fillCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_SHOULD_START);
        tft.drawCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_ROUND);
        break;
      case 101: // must be off, is on, or error
      default:
        tft.fillCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_SHOULD_STOP);
        tft.drawCircle(tft.width()/2, tft.height() - MARGIN_BOTTOM - RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_COMPRESSOR_ROUND);
        break;
    }
    last_compressor_state_must_is = compressor_state_must_is;
  }

  if (select_option && (3 == option)) {

    if ((last_he_valve_state != he_valve_is_on) || (0 == last_he_valve_changed_state)) {
      if (he_valve_is_on) {
        tft.fillCircle(tft.width()/2, MARGIN_TOP + RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_HE);
        tft.drawCircle(tft.width()/2, MARGIN_TOP + RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_HE_DARK);
      } else {
        if (select_option && (3 == option)) {
          total_he_valve_on = total_he_valve_on + now - last_he_valve_changed_state;
        }
        tft.fillCircle(tft.width()/2, MARGIN_TOP + RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_VALVE_OFF);
        tft.drawCircle(tft.width()/2, MARGIN_TOP + RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_HE_DARK);
      }
      last_he_valve_state = he_valve_is_on;
      last_he_valve_changed_state = now;
    }

    if ((last_o2_valve_state != o2_valve_is_on) || (0 == last_he_valve_changed_state)) {
      if (o2_valve_is_on) {
        tft.fillCircle(tft.width()/2, MARGIN_TOP + 4 * RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_O2);
        tft.drawCircle(tft.width()/2, MARGIN_TOP + 4 * RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_O2_DARK);
      } else {
        if (select_option && (3 == option)) {
          total_o2_valve_on = total_o2_valve_on + now - last_o2_valve_changed_state;
        }
        tft.fillCircle(tft.width()/2, MARGIN_TOP + 4 * RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_VALVE_OFF);
        tft.drawCircle(tft.width()/2, MARGIN_TOP + 4 * RUNNING_CIRCLE_RADIUS, RUNNING_CIRCLE_RADIUS, COLOR565_O2_DARK);
      }
      last_o2_valve_state = o2_valve_is_on;
      last_o2_valve_changed_state = now;
    }

    total_fill_time = now - start_fill;
  }

  if (int(now/1000) != last_time_update) {
    tft.setTextColor(COLOR565_TEXT_INFO, COLOR565_BACKGROUND);
    tft.setFreeFont(FF1);
    tft.setTextDatum(MR_DATUM);
    tft.drawString(printLocalTime(), SCREEN_WIDTH - margin_right, between_box_center, GFXFF); // 2 or GFXFF
    last_time_update = int(now/1000);
  }

  if (1 == (int(now / 500) % 2)) {
    // color_running = COLOR565_RUNNING_DARK;
    color_calibration_flashing = color_calibration_current;
    color_fill_flashing = color_fill_current;
  } else {
    // color_running = COLOR565_RUNNING_LIGHT;
    color_calibration_flashing = color_calibration_light;
    color_fill_flashing = color_fill_light;
  }

  running512 = int(now/2) % 512;
  if (running512 >= 256) {
    running512 = 511-running512;
  }
  color_running = tft.color565(0, running512, running512);

  if (color_running != last_color_running) {
    tft.fillCircle(tft.width()/2, between_box_center, RUNNING_CIRCLE_RADIUS, color_running);
    last_color_running = color_running;
  }

  if (ads_available) {
    if (!app_started) {
      DEBUG_PRINTLN("We will now read ads for the first time.");
    }
    steps_for_D2 = ads.readADC_Differential_0_1();
    steps_for_D1 = ads.readADC_Differential_2_3();
  } else {
    steps_for_D2 = 0;
    steps_for_D1 = 0;
  }

  voltage_detector_for_D1 = abs(1000 * ads.computeVolts(steps_for_D1));
  voltage_detector_for_D2 = abs(1000 * ads.computeVolts(steps_for_D2));

  if (debug_mode) {
    voltage_detector_for_D1 = 4;
    voltage_detector_for_D2 = 15;
  }

  percent_D1 = VoltageToPercentO2(voltage_detector_for_D1, voltage_ambiant_air_D1);
  percent_O2 = VoltageToPercentO2(voltage_detector_for_D2, voltage_ambiant_air_D2);

  if ((fixed_ambiant_air_O2 * (100 - percent_D1)) > 0) {
    percent_HE = 100 * ((fixed_ambiant_air_O2 - percent_D1) * (100 - percent_O2)) / (fixed_ambiant_air_O2 * (100 - percent_D1));
  } else {
    percent_HE = 99.9;
  }
  if (percent_HE >= 99.9) {
    percent_HE = 99.9;
  } else if (percent_HE <= 0) {
    percent_HE = 0;
  }

  if (option < 4) { // Only if no alert
    if (rotaryEncoder.encoderChanged() || ws_option_flag) {
      if (ws_option_flag) {
        option = ws_option;
      } else {
        if (!select_option) {
          option = rotaryEncoder.readEncoder();
          if (option > 3) {
            option = 3;
          }
        } else {
          value = rotaryEncoder.readEncoder();
        }
      }
      last_display_update = 0;
      last_probe_update = 0;
     DEBUG_PRINTLN("Option: " + String(option));
    }
  }

  if (rotaryEncoder.isEncoderButtonClicked() || ws_option_flag) {
    if (ws_option_flag) {
      select_option = ws_select_option;
    } else {
      select_option = !select_option;
    }
    if (select_option) {
      tft.drawRect(selected_x[option], selected_y[option], BOX_WIDTH, BOX_HEIGHT, COLOR565_SELECT_BOX);
      rotaryEncoder.setAcceleration(500);
      rotaryEncoder.setBoundaries(0, 1000, false);  // 0-3 with loop
      rotaryEncoder.setEncoderValue(value);

      if (2 == option) {
        start_calibration = now;
      } else if (3 == option) {
        total_fill_time    = 0;
        total_he_valve_on  = 0;
        total_o2_valve_on  = 0;
        last_he_valve_changed_state = now;
        last_o2_valve_changed_state = now;
        start_fill = now;
      }
      if ((3 != option) && (0 != start_fill)) {
        if (he_valve_is_on) {
          total_he_valve_on = total_he_valve_on + now - last_he_valve_changed_state;
        }
        if (o2_valve_is_on) {
          total_o2_valve_on = total_o2_valve_on + now - last_o2_valve_changed_state;
        }
        total_fill_time = now - start_fill;
        start_fill = 0;
      }

    } else {
      tft.drawRect(selected_x[last_option], selected_y[last_option], BOX_WIDTH, BOX_HEIGHT, selected_color[last_option]);
      rotaryEncoder.setAcceleration(0);
      rotaryEncoder.setBoundaries(0, 3, false);  // 0-3 with loop
      rotaryEncoder.setEncoderValue(option);
    }
  }

  ws_option_flag = false;

  if  ((now - last_display_update) > UPDATE_REFRESH) {
    display_update = true;
    last_display_update = now;
  } else {
    display_update = false;
  }

  if (((now - last_calibration_info_update) > CALIBRATION_INFO_REFRESH) || (0 == last_calibration_info_update)) {
    calibration_info_update = true;
    last_calibration_info_update = now;
  } else {
    calibration_info_update = false;
  }

  switch (option) {
    case 0:  // O2 setting
      if (option != last_option) {
        color_o2_current = color_o2_dark;
        color_he_current = color_he;
        color_calibration_current = color_calibration;
        color_fill_current = color_fill;
        color_o2_back = color_o2;
        color_he_back = color_he_dark;
        color_calibration_back = color_calibration_dark;
        color_fill_back = color_fill_dark;

        tft.fillRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2_dark);
        tft.fillRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2);
        tft.fillRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he_dark);
        tft.drawRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he);
        tft.fillRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration_dark);
        tft.drawRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration);
        tft.fillRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill_dark);
        tft.drawRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill);
      }
      break;

    case 1:  // HE setting
      if (option != last_option) {
        color_o2_current = color_o2;
        color_he_current = color_he_dark;
        color_calibration_current = color_calibration;
        color_fill_current = color_fill;
        color_o2_back = color_o2_dark;
        color_he_back = color_he;
        color_calibration_back = color_calibration_dark;
        color_fill_back = color_fill_dark;

        tft.fillRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2_dark);
        tft.drawRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2);
        tft.fillRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he_dark);
        tft.fillRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he);
        tft.fillRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration_dark);
        tft.drawRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration);
        tft.fillRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill_dark);
        tft.drawRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill);
      }
      break;

    case 2:  // Calbration
      if (option != last_option) {
        color_o2_current = color_o2;
        color_he_current = color_he;
        color_calibration_current = color_calibration_dark;
        color_fill_current = color_fill;
        color_o2_back = color_o2_dark;
        color_he_back = color_he_dark;
        color_calibration_back = color_calibration;
        color_fill_back = color_fill_dark;

        tft.fillRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2_dark);
        tft.drawRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2);
        tft.fillRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he_dark);
        tft.drawRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he);
        tft.fillRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration_dark);
        tft.fillRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration);
        tft.fillRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill_dark);
        tft.drawRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill);
      }
      break;

    case 3:  // Run
      if (option != last_option) {
        color_o2_current = color_o2;
        color_he_current = color_he;
        color_calibration_current = color_calibration;
        color_fill_current = color_fill_dark;
        color_o2_back = color_o2_dark;
        color_he_back = color_he_dark;
        color_calibration_back = color_calibration_dark;
        color_fill_back = color_fill;

        tft.fillRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2_dark);
        tft.drawRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2);
        tft.fillRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he_dark);
        tft.drawRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he);
        tft.fillRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration_dark);
        tft.drawRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration);
        tft.fillRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill_dark);
        tft.fillRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill);
      }
      break;

    case 4:  // Alert
      if (option != last_option) {
        color_o2_current = color_o2;
        color_he_current = color_he;
        color_calibration_current = color_calibration;
        color_fill_current = color_fill_dark;
        color_o2_back = color_o2_dark;
        color_he_back = color_he_dark;
        color_calibration_back = color_calibration_dark;
        color_fill_back = color_fill_dark;

        tft.fillRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2_dark);
        tft.drawRect(upper_box_left_left, upper_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_o2);
        tft.fillRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he_dark);
        tft.drawRect(upper_box_right_left, upper_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_he);
        tft.fillRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration_dark);
        tft.drawRect(lower_box_left_left, lower_box_left_top, BOX_WIDTH, BOX_HEIGHT, color_calibration);
        tft.fillRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill_dark);
        tft.fillRect(lower_box_right_left, lower_box_right_top, BOX_WIDTH, BOX_HEIGHT, color_fill);

        tft.setFreeFont(FF23); // Because % is not existing in font size 6, only numbers (like 7 and 8)
        padding = tft.textWidth(String(TXT_O2_ALERT), GFXFF);
        font_height = tft.fontHeight(GFXFF);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(color_fill_current, color_fill_back);
        tft.drawString(String(TXT_O2_ALERT), lower_box_right_center, lower_box_right_middle, GFXFF);
      }
      break;
  }

  if (0 == option) {
    SetCompState(false);
    digitalWrite(RELAY_HE_VALVE, LOW);
    digitalWrite(RELAY_O2_VALVE, LOW);
  }

  if (1 == option) {
    SetCompState(false);
    digitalWrite(RELAY_HE_VALVE, LOW);
    digitalWrite(RELAY_O2_VALVE, LOW);
  }

  if (select_option && (2 == option)) {
    if ((now - start_calibration) <= MAX_CALIBRATION_TIME) {
    } else {
      select_option = false;

      if (debug_mode) {
        voltage_ambiant_air_D1 = DEBUG_D1_VALUE;
        voltage_ambiant_air_D2 = DEBUG_D2_VALUE;
      } else {
        voltage_ambiant_air_D1 = voltage_detector_for_D1;
        voltage_ambiant_air_D2 = voltage_detector_for_D2;
      }

      last_calibration_time_epoch = getTimestamp();
      last_calibration_time = printLocalTime(TXT_FORMAT_CALIBRATION, last_calibration_time_epoch);

      if (spiffs_mounted) {
        File fileHE = SPIFFS.open(spiffs_detector_for_D1_ambiant_air, FILE_WRITE);
        fileHE.print(String(voltage_ambiant_air_D1));
        fileHE.close();

        File fileO2 = SPIFFS.open(spiffs_detector_for_D2_ambiant_air, FILE_WRITE);
        fileO2.print(String(voltage_ambiant_air_D2));
        fileO2.close();

        File fileCT = SPIFFS.open(spiffs_calibration_time, FILE_WRITE);
        fileCT.print(String(last_calibration_time_epoch));
        fileCT.close();
        DEBUG_PRINTLN("Calibration time re-initialized");
      }

      tft.drawRect(selected_x[option], selected_y[option], BOX_WIDTH, BOX_HEIGHT, selected_color[option]);
      rotaryEncoder.setAcceleration(0);
      rotaryEncoder.setBoundaries(0, 3, false);  // 0-3 with loop
      rotaryEncoder.setEncoderValue(option);
    }
    SetCompState(true);
    digitalWrite(RELAY_HE_VALVE, LOW);
    digitalWrite(RELAY_O2_VALVE, LOW);
    if (color_calibration_flashing != last_color_calibration_flashing) {
      tft.setFreeFont(FF23); // Because % is not existing in font size 6, only numbers (like 7 and 8)
      padding = tft.textWidth(String(TXT_CALIBRATE), GFXFF);
      font_height = tft.fontHeight(GFXFF);
      tft.setTextDatum(TC_DATUM);
      tft.setTextColor(color_calibration_flashing, color_calibration_back);
      tft.drawString(String(TXT_CALIBRATE), lower_box_left_center, lower_box_left_top + PADDING_BOX, GFXFF);
      last_color_calibration_flashing = color_calibration_flashing;
    }
  } else if (2 == option) {
    SetCompState(false);
    digitalWrite(RELAY_HE_VALVE, LOW);
    digitalWrite(RELAY_O2_VALVE, LOW);
  }

  if (select_option && (3 == option)) {
    SetCompState(true);

    if (percent_O2 >= (o2mix + percent_hysteresis_o2_high)) {
      digitalWrite(RELAY_O2_VALVE, LOW);
    } else if (percent_O2 <= (o2mix + percent_hysteresis_o2_low)) {
      digitalWrite(RELAY_O2_VALVE, HIGH);
    }

    if (percent_HE >= (hemix + percent_hysteresis_he_high)) {
      digitalWrite(RELAY_HE_VALVE, LOW);
    } else if (percent_HE <= (hemix + percent_hysteresis_he_low)) {
      digitalWrite(RELAY_HE_VALVE, HIGH);
    }

    if (color_fill_flashing != last_color_fill_flashing) {
      tft.setFreeFont(FF23); // Because % is not existing in font size 6, only numbers (like 7 and 8)
      padding = tft.textWidth(String(TXT_FILL), GFXFF);
      font_height = tft.fontHeight(GFXFF);
      tft.setTextDatum(TC_DATUM);
      tft.setTextColor(color_fill_flashing, color_fill_back);
      tft.drawString(String(TXT_FILL), lower_box_right_center, lower_box_right_top + PADDING_BOX, GFXFF);
      last_color_fill_flashing = color_fill_flashing;
    }
  } else if (3 == option) {
    SetCompState(false);
    digitalWrite(RELAY_HE_VALVE, LOW);
    digitalWrite(RELAY_O2_VALVE, LOW);
  }

  if (calibration_info_update) {
    last_sent_debug_mode = 0;
    last_sent_voltage_ambiant_air_D1 = "";
    last_sent_voltage_ambiant_air_D2 = "";
    last_sent_calibration_time = "";
  }

  if (he_valve_is_on) {
    running_he_valve_on = total_he_valve_on + now - last_he_valve_changed_state;
  } else {
    running_he_valve_on = total_he_valve_on;
  }

  if (o2_valve_is_on) {
    running_o2_valve_on = total_o2_valve_on + now - last_o2_valve_changed_state;
  } else {
    running_o2_valve_on = total_o2_valve_on;
  }

  /* Calculate the json string */
  // Send json_out via WebSocket
  json_out.clear();

  json_out["comp"] = String(compressor_state_must_is);

  json_out["hevalve"] = String(he_valve_is_on ? STATE_ON : STATE_OFF);
  json_out["o2valve"] = String(o2_valve_is_on ? STATE_ON : STATE_OFF);

  json_out["hemv"] = String(dtostrf(voltage_detector_for_D1,4,2,tmp));
  json_out["o2mv"] = String(dtostrf(voltage_detector_for_D2,4,2,tmp));

  json_out["he"]   = String(dtostrf(percent_HE,3,1,tmp));
  json_out["o2"]   = String(dtostrf(percent_O2,3,1,tmp));

  json_out["time"] = String(printLocalTime());

  json_out["op"] = String(option);
  json_out["so"] = String(select_option ? 1 : 0);

  if (last_sent_debug_mode != debug_mode) {
    json_out["debug"] = String(debug_mode ? 1 : 0);
    last_sent_debug_mode = debug_mode;
  }

  if (select_option && (3 == option)) {
    json_out["heon"] = String(running_he_valve_on);
    json_out["o2on"] = String(running_o2_valve_on);
    json_out["tf"] = String(total_fill_time);
  }

  if (last_sent_voltage_ambiant_air_D1 != String(dtostrf(voltage_ambiant_air_D1,3,2,tmp))) {
    last_sent_voltage_ambiant_air_D1 = String(dtostrf(voltage_ambiant_air_D1,3,2,tmp));
    json_out["d1c"] = last_sent_voltage_ambiant_air_D1;
  }
  if (last_sent_voltage_ambiant_air_D2 != String(dtostrf(voltage_ambiant_air_D2,3,2,tmp))) {
    last_sent_voltage_ambiant_air_D2 = String(dtostrf(voltage_ambiant_air_D2,3,2,tmp));
    json_out["d2c"] = last_sent_voltage_ambiant_air_D2;
  }
  if (last_sent_calibration_time != last_calibration_time) {
    json_out["cd"] = String(last_calibration_time);
    last_sent_calibration_time = last_calibration_time;
  }
  serializeJson(json_out, payload);

  if (((now - last_probe_update) > MAX_PROBE_REFRESH) && (0 != strncmp(payload, last_payload, JSON_MAX_SIZE))) {
    last_probe_update = now;
    // DEBUG_PRINT(">");

    if (ws.count()) {
      ws.textAll(payload);
      strcpy(last_payload, payload);
      // DEBUG_PRINTF("[WS] send:%s\n", payload);
    }
  }

  if (display_update) {
    DEBUG_PRINT("*");
    if (select_option && (2 == option)) {
      tft.setFreeFont(FF23);
      font_height = tft.fontHeight(GFXFF);
      padding = tft.textWidth(String(TXT_D1_PROBE) + ": " + String(TXT_FORMAT_MILLIVOLT) + String(TXT_MILLIVOLT), FONT_SIZE_INFO_CALIBRATION);
      tft.setTextPadding(padding);
      tft.setTextDatum(TR_DATUM);
      tft.setTextColor(color_calibration_current, color_calibration_back);
      tft.drawString(String(TXT_D1_PROBE) + ": " + String(dtostrf(voltage_detector_for_D1,3,2,tmp)) + String(TXT_MILLIVOLT), lower_box_left_left + (BOX_WIDTH + padding)/2, lower_box_left_top + PADDING_BOX + font_height, FONT_SIZE_INFO_CALIBRATION);
      font_height2 = tft.fontHeight(FONT_SIZE_INFO_CALIBRATION);
      padding = tft.textWidth(String(TXT_D2_PROBE) + ": " + String(TXT_FORMAT_MILLIVOLT) + String(TXT_MILLIVOLT), FONT_SIZE_INFO_CALIBRATION);
      tft.setTextPadding(padding);
      tft.setTextDatum(TR_DATUM);
      tft.setTextColor(color_calibration_current, color_calibration_back);
      tft.drawString(String(TXT_D2_PROBE) + ": " + String(dtostrf(voltage_detector_for_D2,3,2,tmp)) + String(TXT_MILLIVOLT), lower_box_left_left + (BOX_WIDTH + padding)/2, lower_box_left_top + PADDING_BOX + font_height + font_height2, FONT_SIZE_INFO_CALIBRATION);

      if (!ads_available) {
        padding = tft.textWidth(String(TXT_NO_ADS_DETECTED), FONT_SIZE_INFO_CALIBRATION);
        tft.setTextPadding(padding);
        tft.setTextDatum(TR_DATUM);
        tft.setTextColor(COLOR565_ALERT, color_calibration_back);
        tft.drawString(String(TXT_NO_ADS_DETECTED), lower_box_left_left + (BOX_WIDTH + padding)/2, lower_box_left_top + PADDING_BOX + font_height + (2 * font_height2) + 2, FONT_SIZE_INFO_CALIBRATION);
      }

    } else {
      tft.setFreeFont(FF23); // Because % is not existing in font size 6, only numbers (like 7 and 8)
      padding = tft.textWidth(String(TXT_CALIBRATE), GFXFF);
      font_height = tft.fontHeight(GFXFF);
      tft.setTextDatum(TC_DATUM);
      tft.setTextColor(color_calibration_current, color_calibration_back);
      tft.drawString(String(TXT_CALIBRATE), lower_box_left_center, lower_box_left_top + PADDING_BOX, GFXFF);

      padding = tft.textWidth(String(TXT_D1_PROBE) + ": " + String(TXT_FORMAT_MILLIVOLT) + (TXT_MILLIVOLT), FONT_SIZE_INFO_CALIBRATION);
      tft.setTextPadding(padding);
      tft.setTextDatum(TR_DATUM);
      tft.setTextColor(color_calibration_current, color_calibration_back);
      tft.drawString(String(TXT_D1_PROBE) + ": " + String(dtostrf(voltage_ambiant_air_D1,3,2,tmp)) + String(TXT_MILLIVOLT), lower_box_left_left + (BOX_WIDTH + padding)/2, lower_box_left_top + PADDING_BOX + font_height, FONT_SIZE_INFO_CALIBRATION);
      font_height2 = tft.fontHeight(FONT_SIZE_INFO_CALIBRATION);
      padding = tft.textWidth(String(TXT_D2_PROBE) + ": " + String(TXT_FORMAT_MILLIVOLT) + String(TXT_MILLIVOLT), FONT_SIZE_INFO_CALIBRATION);
      tft.setTextPadding(padding);
      tft.setTextDatum(TR_DATUM);
      tft.setTextColor(color_calibration_current, color_calibration_back);
      tft.drawString(String(TXT_D2_PROBE) + ": " + String(dtostrf(voltage_ambiant_air_D2,3,2,tmp)) + String(TXT_MILLIVOLT), lower_box_left_left + (BOX_WIDTH + padding)/2, lower_box_left_top + PADDING_BOX + font_height + font_height2, FONT_SIZE_INFO_CALIBRATION);

      if (!ads_available) {
        padding = tft.textWidth(String(TXT_NO_ADS_DETECTED), FONT_SIZE_INFO_CALIBRATION);
         tft.setTextPadding(padding);
        tft.setTextDatum(TR_DATUM);
        tft.setTextColor(COLOR565_ALERT, color_calibration_back);
        tft.drawString(String(TXT_NO_ADS_DETECTED), lower_box_left_left + (BOX_WIDTH + padding)/2, lower_box_left_top + PADDING_BOX + font_height + (2 * font_height2) + 2, FONT_SIZE_INFO_CALIBRATION);
      }

    }

    if (select_option && (3 == option)) {
      tft.setFreeFont(FF23); // Because % is not existing in font size 6, only numbers (like 7 and 8)
      font_height = tft.fontHeight(GFXFF);
      padding = tft.textWidth(String(TXT_FILL_TOTAL_TIME) + ": 9999 " + String(TXT_SECONDS), FONT_SIZE_PROBE_MV);
      if (select_option && (3 == option)) {
        tft.setTextPadding(padding);
        tft.setTextDatum(TC_DATUM);
        if (int(total_fill_time/1000) > 0) {
          tft.setTextColor(color_fill_current, color_fill_back);
          tft.drawString(String(TXT_FILL_TOTAL_TIME) + ": " + String(dtostrf(int(total_fill_time/1000),1,0,tmp)) + " " + String(TXT_SECONDS), lower_box_right_center, lower_box_right_top + PADDING_BOX + font_height, FONT_SIZE_TOTAL_TIME);
        } else {
          tft.setTextColor(color_fill_current, color_fill_back);
          tft.drawString("", lower_box_right_center, lower_box_right_top + PADDING_BOX + font_height, GFXFF);
        }
      }
    } else if (option < 4) {
      tft.setFreeFont(FF23); // Because % is not existing in font size 6, only numbers (like 7 and 8)
      padding = tft.textWidth(String(TXT_FILL), GFXFF);
      font_height = tft.fontHeight(GFXFF);
      tft.setTextDatum(TC_DATUM);
      tft.setTextColor(color_fill_current, color_fill_back);
      tft.drawString(String(TXT_FILL), lower_box_right_center, lower_box_right_top + PADDING_BOX, GFXFF);
    }

    if (last_calibration_time.length() > 0) {
      tft.setTextColor(COLOR565_TEXT_INFO, color_calibration_back);
      tft.setTextDatum(BC_DATUM);
      tft.drawString(String(TXT_CALIBRATED) + ": " + last_calibration_time, lower_box_left_center, lower_box_left_top + BOX_HEIGHT - PADDING_BOX, 2);
    }

    // DEBUG
    /*
    if (select_option && (3 == option)) {
      voltage_detector_for_D2 = voltage_detector_for_D2 + ((now - start_fill) / 1000);
    }
    */

    tft.setFreeFont(FF24); // Because % is not existing in font size 6, only numbers (like 7 and 8)
    padding = tft.textWidth(String(TXT_FORMAT_PERCENT) + "%", GFXFF);
    font_height = tft.fontHeight(GFXFF);
    tft.setTextPadding(padding);
    tft.setTextDatum(TR_DATUM);
    tft.setTextColor(color_o2_current, color_o2_back);
    tft.drawString(String(dtostrf(percent_O2,3,1,tmp)) + "%", upper_box_left_left + BOX_WIDTH - PADDING_BOX, MARGIN_HEIGHT + PADDING_BOX, GFXFF);
    tft.setTextColor(color_he_current, color_he_back);
    tft.drawString(String(dtostrf(percent_HE,3,1,tmp)) + "%", upper_box_right_left + BOX_WIDTH - PADDING_BOX, MARGIN_HEIGHT + PADDING_BOX, GFXFF);

    tft.setFreeFont(FF23);
    font_height2 = tft.fontHeight(GFXFF);
    tft.setTextPadding(0);
    tft.setTextDatum(BL_DATUM);
    tft.setTextColor(color_he_current, color_he_back);
    tft.drawString("He", upper_box_right_left + PADDING_BOX, MARGIN_HEIGHT + PADDING_BOX + (font_height/1.3), GFXFF);
    tft.setTextColor(color_o2_current, color_o2_back);
    tft.drawString("O", upper_box_left_left + PADDING_BOX, MARGIN_HEIGHT + PADDING_BOX + (font_height/1.3), GFXFF);
    font_width = tft.textWidth("O", GFXFF);
    tft.setFreeFont(FF22);
    tft.drawString("2", upper_box_left_left + PADDING_BOX + font_width, MARGIN_HEIGHT + PADDING_BOX +  (font_height/1.3) + (font_height2/4), GFXFF);

    /*
    padding = tft.textWidth(String(TXT_FORMAT_MILLIVOLT) + String(TXT_MILLIVOLT), FONT_SIZE_PROBE_MV);
    tft.setTextPadding(padding);
    tft.setTextDatum(TC_DATUM);
    tft.setTextColor(color_o2_current, color_o2_back);
    tft.drawString("(" + String(dtostrf(voltage_detector_for_D2,3,2,tmp)) + String(TXT_MILLIVOLT) + ")", upper_box_left_center, upper_box_left_top + PADDING_BOX + font_height, FONT_SIZE_PROBE_MV);
    tft.setTextColor(color_he_current, color_he_back);
    tft.drawString("(" + String(dtostrf(voltage_detector_for_D1,3,2,tmp)) + String(TXT_MILLIVOLT) + ")", upper_box_right_center, upper_box_right_top + PADDING_BOX + font_height, FONT_SIZE_PROBE_MV);
    */
    padding = tft.textWidth("99% ON, 99% OFF", FONT_SIZE_PROBE_MV);
    if (select_option && (3 == option)) {
      tft.setTextPadding(padding);
      tft.setTextDatum(TC_DATUM);
      if (int(total_fill_time/1000) > 0) {
        tft.setTextColor(color_o2_current, color_o2_back);
        tft.drawString(String(dtostrf(int(0.5 + (100.0 * running_o2_valve_on)/total_fill_time),1,0,tmp)) + "% ON, " + String(dtostrf(100-int(0.5 + (100.0 * running_o2_valve_on)/total_fill_time),1,0,tmp)) + "% OFF", upper_box_left_center, upper_box_left_top + PADDING_BOX + font_height - 5, FONT_SIZE_VALVE_PERCENT);
        tft.setTextColor(color_he_current, color_he_back);
        tft.drawString(String(dtostrf(int(0.5 + (100.0 * running_he_valve_on)/total_fill_time),1,0,tmp)) + "% ON, " + String(dtostrf(100-int(0.5 + (100.0 * running_he_valve_on)/total_fill_time),1,0,tmp)) + "% OFF", upper_box_right_center, upper_box_right_top + PADDING_BOX + font_height - 5, FONT_SIZE_VALVE_PERCENT);
      } else {
        tft.setTextColor(color_o2_current, color_o2_back);
        tft.drawString("", upper_box_left_center, upper_box_left_top + PADDING_BOX + font_height, FONT_SIZE_PROBE_MV);
        tft.setTextColor(color_he_current, color_he_back);
        tft.drawString("", upper_box_right_center, upper_box_right_top + PADDING_BOX + font_height, FONT_SIZE_PROBE_MV);
      }
    }

    tft.setFreeFont(FF22); // Because % is not existing in font size 6, only numbers (like 7 and 8)
    padding = tft.textWidth(String(TXT_SETPOINT) + "00.00%", GFXFF);
    tft.setTextPadding(padding);
    tft.setTextDatum(BC_DATUM);
    tft.setTextColor(color_o2_current, color_o2_back);
    tft.drawString(String(TXT_SETPOINT) + ": " + String(dtostrf(o2mix,3,1,tmp)) + "%", upper_box_left_center, upper_box_left_top + BOX_HEIGHT - PADDING_BOX, GFXFF);
    tft.setTextColor(color_he_current, color_he_back);
    tft.drawString(String(TXT_SETPOINT) + ": " + String(dtostrf(hemix,3,1,tmp)) + "%", upper_box_right_center, upper_box_right_top + BOX_HEIGHT - PADDING_BOX, GFXFF);
  
  }

  last_option = option;
  if (select_option) {
    last_selected_option = option;
  }

  // O2 alert detection, not during calibration, and only if at least one valve is open or the compressor is running
  // !!! We change the current option !!!
  if ((percent_O2 >= O2_PERCENT_ALERT) && !(select_option && (2 == option))) {
    if ((!digitalRead(COMPRESSOR_STATE_PIN)) || digitalRead(RELAY_HE_VALVE) || digitalRead(RELAY_O2_VALVE)) {
      DEBUG_PRINTLN("Alert detect !");
      DEBUG_PRINTLN("Percent " + String(percent_O2));
      DEBUG_PRINTLN("Percent alert " + String(O2_PERCENT_ALERT));
      option = 4;
      SetCompState(false);
      digitalWrite(RELAY_HE_VALVE, LOW);
      digitalWrite(RELAY_O2_VALVE, LOW);
    }
  }

  if (now < MDNS_TIMEOUT) {
    mDNSUpdate();
  }
  app_started = true;
}

String formatTimeDigits(int num) {
  char strOut[3];
  strOut[0] = '0' + (num / 10);
  strOut[1] = '0' + (num % 10);
  strOut[2] = '\0';
  return strOut;
}

time_t getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    DEBUG_PRINTLN("Failed to obtain time");
    configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER);
    return 0;
  }
  return mktime(&timeinfo);
}

String printLocalTime(const char* time_format) {
  String time_str;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    DEBUG_PRINTLN("Failed to obtain time");
    configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER);
    return String(ERR_TIME_ERROR);
  }
  //See http://www.cplusplus.com/reference/ctime/strftime/
  char output[80];
  strftime(output, 80, time_format, &timeinfo);
  time_str = String(output);
  return String(output);
}

String printLocalTime(const char* time_format, time_t epoch_time) {
  String time_str;
  struct tm* timeinfo;
  timeinfo = localtime(&epoch_time);
  char output[80];
  strftime(output, 80, time_format, timeinfo);
  time_str = String(output);
  return String(output);
}

String printLocalTime() {
  return printLocalTime(TXT_FORMAT_TIME_FULL);
}

String printLocalDate() {
  return printLocalTime(TXT_FORMAT_DATE);
}

float VoltageToPercentO2(float voltage_value, float voltage_ambiant_air) {
  float percent_o2 = (fixed_ambiant_air_O2 * voltage_value) / voltage_ambiant_air;
  if (percent_o2 < 0) {
    percent_o2 = 0;
  } else if (percent_o2 > 99.9) {
    percent_o2 = 99.9;
  }
  return percent_o2;
}

float VoltageToPercentHE(float voltage_value, float voltage_ambiant_air) {
  float percent_he = 100 - (100 * voltage_value / voltage_ambiant_air);
  if (percent_he < 0) {
    percent_he = 0;
  } else if (percent_he > 99.9) {
    percent_he = 99.9;
  }
  return percent_he;
}

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[PNG_MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(0, 0 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

// Launch of mDNS service
void startMDNS(void) {
  MDNS.end();
  // const char* hostname = WiFi.getHostname();
  DEBUG_PRINTF("mDNS responder %s.local ", hostname);
  if (MDNS.begin(hostname)) {
    DEBUG_PRINT("service ");
    if (MDNS.addService("http", "tcp", httpPort))
      DEBUG_PRINTLN("started");
    else
      DEBUG_PRINTLN("failed");
  }
  else
    DEBUG_PRINTLN("could not start");
}
