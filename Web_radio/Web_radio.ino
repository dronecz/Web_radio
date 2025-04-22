
/*******************************************************************************

Sketch settings for ESP32-S3 Dev module: 

- USB CDC on Boot: "Enabled"
- JTAG adatpeer: "Integrated USB JTAG" (additional settings needed)
- Partition scheme: "Huge app (3MB app, no OTA/1 MB SPIFFS)"
- PSRAM: "QSPI PSRAM"

 ******************************************************************************/

/*
 https://github.com/genvex/LVGL_Audio_FFT_Spectrum_xiaoS3_oled
 https://github.com/moononournation/LVGL_Music_Player/blob/main/Winamp480x320/Winamp480x320.ino - https://www.instructables.com/Design-Music-Player-UI-With-LVGL/
 https://www.robinscheibler.org/2017/12/12/esp32-fft.html
 https://www.elektormagazine.com/articles/fast-fourier-transform-fft-on-the-esp32
 Google -> ESP32-audioI2S fft

 https://youtu.be/IslG_mzpc1g
 
 */
#include <Arduino_GFX_Library.h>
#include "arduino_secrets.h"
#include "Audio.h"
#include <lvgl.h>
#include "src/ui/ui.h"
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

#define DIRECT_MODE  // Uncomment to enable full frame buffer

// Connections ESP32S3 <-> Amplifier
#define I2S_DOUT 9
#define I2S_BCLK 3
#define I2S_LRC 1
Audio audio;

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_ESP32SPI(41 /* DC */, 40 /* CS */, 12 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, FSPI /* spi_num */);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ST7796(bus, 42 /* RST */, 1 /* rotation */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
   End of Arduino_GFX setting
 ******************************************************************************/

/* Change to your screen resolution */
uint32_t screenWidth;
uint32_t screenHeight;
uint32_t bufSize;
lv_display_t *disp;
lv_color_t *disp_draw_buf;

#include "src\FFT.h"
#define CANVAS_FFT_WIDTH 400  //screenWidth
#define CANVAS_FFT_HEIGHT 80
Arduino_Canvas *canvasFFT_gfx = new Arduino_Canvas(CANVAS_FFT_WIDTH /* width */, CANVAS_FFT_HEIGHT /* height */, NULL);
lv_obj_t *ui_CanvasFFT;

static unsigned long targetAnimTime, targetCountTime;
int year, month, day, hour, minutes, sec = 0;

// use 8 bit precision for LEDC timer
#define LEDC_TIMER_8_BIT 8

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ 5000

// LED channel that will be used instead of automatic selection.
#define LEDC_CHANNEL 0

int brightness = 32;  // initial brightness of the screen 0 - 255

int btnPin[4] = { 7, 10, 16, 17 };
int lastButtonNum;

// Variables will change:
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;   // the debounce time; increase if the output flickers

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

uint32_t millis_cb(void) {
  return millis();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
#ifndef DIRECT_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif  // #ifndef DIRECT_MODE

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

void countTime() {
  unsigned long currMillisCountTime = millis();
  char numberString[2];
  if (currMillisCountTime >= targetCountTime) {
    targetCountTime += 1000;
    sec++;  // Advance second
    if (sec >= 60) {
      sec = 0;
      minutes++;  // Advance minutes
      sprintf(numberString, "%02d", minutes);
      lv_label_set_text(ui_LblMin, numberString);
      if (minutes >= 60) {
        minutes = 0;
        hour++;  // Advance hour
        if (hour >= 24) {
          hour = 0;
        }
        sprintf(numberString, "%02d", hour);
        lv_label_set_text(ui_LblHrs, numberString);
      }
    }
    //Serial.println("Time is " + String(hh) + (":") + String(mm) + (":") + String(ss));
  }
}


void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

uint8_t getBtn() {
  bool digRead[4];
  uint8_t dataRead = 0;
  digRead[0] = digitalRead(btnPin[0]);
  digRead[1] = digitalRead(btnPin[1]);
  digRead[2] = digitalRead(btnPin[2]);
  digRead[3] = digitalRead(btnPin[3]);

  if (digRead[0] == 0) {  // Select button
    dataRead = 1;
  }
  if (digRead[1] == 0) {  // Back button
    dataRead = 2;
  }
  if (digRead[2] == 0) {  // Menu button
    dataRead = 3;
  }
  if (digRead[3] == 0) {  // Menu button
    dataRead = 4;
  }

  return dataRead;
}

// This code is based on Debounce sketch provided by Arduino IDE
void readButtons() {

  // Read the state of the button
  uint8_t reading = getBtn();

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonNum) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState != 0) {
        Serial.println("Buttonn nm." + String(buttonState) + " was pressed!");
        //changeScreen(buttonState);
      }
      //buttonEvent(reading);
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonNum:
  lastButtonNum = reading;
}

// Print station info
void audio_info(const char *info) {
  Serial.print("info        ");
  Serial.println(info);
}
void audio_id3data(const char *info) {  //id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
}
void audio_eof_mp3(const char *info) {  //end of file
  Serial.print("eof_mp3     ");
  Serial.println(info);
}
void audio_showstation(const char *info) {
  Serial.print("station     ");
  Serial.println(info);
  lv_label_set_text(ui_LblStation, info);
}
void audio_showstreaminfo(const char *info) {
  Serial.print("streaminfo  ");
  Serial.println(info);
}
void audio_showstreamtitle(const char *info) {
  Serial.print("streamtitle ");
  Serial.println(info);
  lv_label_set_text(ui_LblCurPlaying, info);
}
void audio_bitrate(const char *info) {
  Serial.print("bitrate     ");
  Serial.println(info);
}
void audio_commercial(const char *info) {  //duration in sec
  Serial.print("commercial  ");
  Serial.println(info);
}
void audio_icyurl(const char *info) {  //homepage
  Serial.print("icyurl      ");
  Serial.println(info);
}
void audio_lasthost(const char *info) {  //stream URL played
  Serial.print("lasthost    ");
  Serial.println(info);
}
void audio_eof_speech(const char *info) {
  Serial.print("eof_speech  ");
  Serial.println(info);
}

//void audio_process_i2s(int16_t* outBuff, uint16_t validSamples, uint8_t bitsPerSample, uint8_t channels, bool *continueI2S) {
  // raw_data[raw_data_idx++] = *validSamples;
  // if (raw_data_idx >= WAVE_SIZE) {
  //   Serial.println(String(raw_data_idx));
  //   fft.exec((int16_t *)raw_data);
  //   draw_fft_level_meter(canvasFFT_gfx);
  //   lv_obj_invalidate(ui_CanvasFFT);
  //   raw_data_idx = 0;
//   }
//   *continueI2S = true;
// }

// void audio_process_i2s(int16_t* outBuff, uint16_t validSamples, uint8_t bitsPerSample, uint8_t channels, bool *continueI2S){

//     int16_t sineWaveTable[44] = {
//          0,   3743,   7377,  10793,  14082,  17136,  19848,  22113,  23825,  24908,
//       25311,  24908,  23825,  22113,  19848,  17136,  14082,  10793,   7377,   3743,
//          0,  -3743,  -7377, -10793, -14082, -17136, -19848, -22113, -23825, -24908,
//      -25311, -24908, -23825, -22113, -19848, -17136, -14082, -10793,  -7377,  -3743
//     };

//     static uint8_t tabPtr = 0;
//     int16_t* sample[2]; // assume 2 channels, 16bit
//     for(int i= 0; i < validSamples; i++){
//         *(sample + 0) = outBuff + i * 2;     // channel left
//         *(sample + 1) = outBuff + i * 2 + 1; // channel right

//         *(*sample + 0) = (sineWaveTable[tabPtr] /50 + *(*sample + 0));
//         *(*sample + 1) = (sineWaveTable[tabPtr] /50 + *(*sample + 1));
//         tabPtr++;
//         if(tabPtr == 44) tabPtr = 0;
//     }
//    *continueI2S = true;
// }

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);

  pinMode(btnPin[0], INPUT_PULLUP);
  pinMode(btnPin[1], INPUT_PULLUP);
  pinMode(btnPin[2], INPUT_PULLUP);
  pinMode(btnPin[3], INPUT_PULLUP);

  Serial.println("Arduino_GFX LVGL_Arduino_v9 example ");
  String LVGL_Arduino = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  // Volume (0-100)
  audio.setVolume(10);

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  // Use single LEDC channel 0
  ledcAttachChannel(GFX_BL, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT, LEDC_CHANNEL);
#endif

  // set notification call-back function
  //sntp_set_time_sync_notification_cb(timeavailable);

  /**
     * NTP server address could be aquired via DHCP,
     *
     * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
     * otherwise SNTP option 42 would be rejected by default.
     * NOTE: configTime() function call if made AFTER DHCP-client run
     * will OVERRIDE aquired NTP server address
     */
  //sntp_servermode_dhcp(1);  // (optional)

  /**
     * This will set configured ntp servers and constant TimeZone/daylightOffset
     * should be OK if your time zone does not need to adjust daylightOffset twice a year,
     * in such a case time adjustment won't be handled automagicaly.
     */
  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  /**
     * A more convenient approach to handle TimeZones with daylightOffset
     * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
     * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
     */
  //configTzTime(time_zone, ntpServer1, ntpServer2);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);

  WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*
        Sync time with NTP server and update ESP32 RTC
        getLocalTime() return false if time is not set
    */
  Serial.println("Syncing time with NTP server..");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(500);
    Serial.print("");
  }

  year = timeinfo.tm_year + 1900;
  month = timeinfo.tm_mon + 1;
  day = timeinfo.tm_mday;
  hour = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  sec = timeinfo.tm_sec;

  Serial.println("");
  printLocalTime();

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  screenWidth = gfx->width();
  screenHeight = gfx->height();

#ifdef DIRECT_MODE
  bufSize = screenWidth * screenHeight;
#else
  bufSize = screenWidth * 40;
#endif

#ifdef ESP32
#if defined(DIRECT_MODE) && (defined(CANVAS) || defined(RGB_PANEL))
  disp_draw_buf = (lv_color_t *)gfx->getFramebuffer();
#else   // !(defined(DIRECT_MODE) && (defined(CANVAS) || defined(RGB_PANEL)))
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf) {
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
#endif  // !(defined(DIRECT_MODE) && (defined(CANVAS) || defined(RGB_PANEL)))
#else   // !ESP32
  Serial.println("LVGL disp_draw_buf heap_caps_malloc failed! malloc again...");
  disp_draw_buf = (lv_color_t *)malloc(bufSize * 2);
#endif  // !ESP32
  if (!disp_draw_buf) {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  } else {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
#ifdef DIRECT_MODE
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
  }

  ui_init();

  // create lvgl canvas to draw FFT
  // canvasFFT_gfx->begin();

  // /*Create a buffer for the canvas*/

  // LV_DRAW_BUF_DEFINE_STATIC(draw_buff, CANVAS_FFT_WIDTH, CANVAS_FFT_HEIGHT, LV_COLOR_FORMAT_RGB565);
  // LV_DRAW_BUF_INIT_STATIC(draw_buff);

  // // /*Create a canvas and initialize its palette*/

  // //ui_CanvasFFT = lv_canvas_create(ui_CntnrRadio);
  // ui_CanvasFFT = lv_canvas_create(lv_scr_act());
  // lv_canvas_set_buffer(ui_CanvasFFT, (lv_color_t *)canvasFFT_gfx->getFramebuffer(), CANVAS_FFT_WIDTH, CANVAS_FFT_HEIGHT, LV_COLOR_FORMAT_RGB565);
  // //lv_canvas_set_draw_buf(ui_CanvasFFT, &draw_buff);

  // //lv_canvas_set_px(ui_CanvasFFT, 0, 440, lv_color_black(), LV_OPA_COVER);
  // lv_canvas_fill_bg(ui_CanvasFFT, lv_color_hex3(0xccc), LV_OPA_TRANSP);
  // //lv_obj_move_foreground(ui_CanvasFFT);
  // //lv_obj_remove_style_all(ui_CntnrVisualisation);

  // lv_obj_set_width(ui_CanvasFFT, CANVAS_FFT_WIDTH);
  // lv_obj_set_height(ui_CanvasFFT, CANVAS_FFT_HEIGHT);
  // lv_obj_set_x(ui_CanvasFFT, 0);
  // lv_obj_set_y(ui_CanvasFFT, 80);
  // lv_obj_set_align(ui_CanvasFFT, LV_ALIGN_CENTER);

  // //lv_obj_center(ui_CanvasFFT);

  // lv_layer_t layer;
  // lv_canvas_init_layer(ui_CanvasFFT, &layer);
  // lv_canvas_finish_layer(ui_CanvasFFT, &layer);

  char numberMin[2], numberHrs[2];
  char numberDate[9];
  sprintf(numberDate, "%02d.%02d.%04d", day, month, year);
  lv_label_set_text(ui_LblDate, numberDate);
  sprintf(numberMin, "%02d", minutes);
  lv_label_set_text(ui_LblMin, numberMin);
  sprintf(numberHrs, "%02d", hour);
  lv_label_set_text(ui_LblHrs, numberHrs);

  // Radio stream, e.g. Byte.fm
  //audio.connecttohost("http://www.byte.fm/stream/bytefm.m3u");
  audio.connecttohost("http://rozhlas.stream/radiozurnal_mp3_128.mp3");

  Serial.println("Setup done");
}

void loop() {
  audio.loop();
  // set the brightness on LEDC channel 0
  //ledcWriteChannel(LEDC_CHANNEL, brightness);

  lv_task_handler(); /* let the GUI do its work */
  readButtons();
  countTime();


#ifdef DIRECT_MODE
#if defined(CANVAS) || defined(RGB_PANEL)
  gfx->flush();
#else   // !(defined(CANVAS) || defined(RGB_PANEL))
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)disp_draw_buf, screenWidth, screenHeight);
#endif  // !(defined(CANVAS) || defined(RGB_PANEL))
#else   // !DIRECT_MODE
#ifdef CANVAS
  gfx->flush();
#endif
#endif  // !DIRECT_MODE

  vTaskDelay(1);
}


//**************************************************************************************************
// DISPLAY SPECTRUM *
//**************************************************************************************************
// *
//**************************************************************************************************
/*

https://github.com/blotfi/ESP32-Radio-with-Spectrum-analyzer/blob/master/src/Esp32_radio.cpp

uint8_t bands = 14; // Number of bands Spectrum Analyzer
uint8_t prevbands = 0; // Number previous band Spectrum Analyzer
uint8_t spectrum[14][3]; // Array per Spectrum Analyzer
const uint8_t Spectrum_y0 = 150;
const uint8_t Spectrum_hy = 70;

void displaySpectrum() {
    if (bands<=0 || bands>14)  return;
    uint8_t larg = dsp_getwidth() / bands-2;
    uint16_t  posi = 5; // start location of the first bar
    boolean visual = true;
//    if (enc_menu_mode == MENU)
//    {
//        visual = false;
//    }
    if (bands != prevbands) {
        prevbands = bands;
    if (visual)
        dsp_fillRect (0, Spectrum_y0, dsp_getwidth(), Spectrum_hy + 1, BLACK);
    }
    for (uint8_t i = 0; i < bands; i++) // Handle all sections
    {
        if (visual) {
            if (spectrum[i][0] > spectrum[i][1]) {
                dsp_fillRect (posi, Spectrum_y0 + Spectrum_hy - spectrum[i][0], larg, spectrum[i][0], GREEN);
                dsp_fillRect (posi, Spectrum_y0, larg, Spectrum_hy - spectrum[i][0], BLACK);
            } else
                dsp_fillRect (posi, Spectrum_y0, larg, Spectrum_hy - spectrum[i][0], BLACK);
        }
        if (spectrum[i][2] > 0) spectrum[i][2]--;
        if (spectrum[i][0] > spectrum[i][2]) spectrum[i][2] = spectrum[i][0];
        if (visual)
            dsp_fillRect (posi, Spectrum_y0 + Spectrum_hy - spectrum[i][2] - 3, larg, 2, RED);
        spectrum[i][1] = spectrum[i][0];
        posi += larg + 2;
    }
}

*/
