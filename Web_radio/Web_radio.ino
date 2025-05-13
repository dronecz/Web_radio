
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
#define I2S_DOUT 10
#define I2S_BCLK 3
#define I2S_LRC 1
Audio audio;

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_ESP32SPI(17 /* DC */, 15 /* CS */, 12 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, FSPI /* spi_num */);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ST7796(bus, 18 /* RST */, 1 /* rotation */);

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
// --- LVGL canvas ---
#define CANVAS_WIDTH 385
#define CANVAS_HEIGHT 40
static lv_obj_t *canvas;
static lv_color_t *canvas_buf;

#define NUM_BARS 35
#define BAR_WIDTH (CANVAS_WIDTH / NUM_BARS)
#define SAMPLE_RATE 44100
static uint32_t fft_auto_max = 10000;
float fft_magnitudes[FFT_SIZE / 2] = { 0 };

// --- FFT ---
#define FFT_SIZE 512
int16_t fft_buffer[FFT_SIZE * 2];
size_t fft_index = 0;
bool ready_to_fft = false;

static unsigned long targetCountTime;
int year, month, day, hour, minutes, sec = 0;

// use 8 bit precision for LEDC timer
#define LEDC_TIMER_8_BIT 8

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ 5000

// LED channel that will be used instead of automatic selection.
#define LEDC_CHANNEL 0

int brightness = 32;  // initial brightness of the screen 0 - 255

const int buttonCount = 5;
const int buttonPins[buttonCount] = { 4, 5, 6, 7, 16 };

bool currentStates[buttonCount];
bool previousStates[buttonCount];
unsigned long lastDebounceTime[buttonCount];
const unsigned long debounceDelay = 50;

int activeButton = -1;

lv_obj_t *lvglButtons[buttonCount];

// Ukazatele na LVGL tlačítka (musí být deklarovány někde jinde)
extern lv_obj_t *ui_Button1;
extern lv_obj_t *ui_Button2;
extern lv_obj_t *ui_Button3;
extern lv_obj_t *ui_Button4;
extern lv_obj_t *ui_Button5;

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

void initButtons() {
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    currentStates[i] = digitalRead(buttonPins[i]);
    previousStates[i] = currentStates[i];
    lastDebounceTime[i] = 0;
  }
}

void processButtons() {
  unsigned long currentTime = millis();

  for (int i = 0; i < buttonCount; i++) {
    int reading = digitalRead(buttonPins[i]);

    if (reading != previousStates[i]) {
      lastDebounceTime[i] = currentTime;
    }

    if ((currentTime - lastDebounceTime[i]) > debounceDelay) {
      if (reading != currentStates[i]) {
        currentStates[i] = reading;

        if (currentStates[i] == LOW) {
          activeButton = i;

          // remove "checked" state from all buttons
          for (int j = 0; j < buttonCount; j++) {
              //find ui_Container[n] as children of ui_Buttons[n], "0" means first child of the object
              lv_obj_t * container = lv_obj_get_child(lvglButtons[j], 0);
              _ui_state_modify(container, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
          }
          // check if lvglButtons[i] is not empty
          if (lvglButtons[i] != nullptr) {
            // send "click" event to the right button
            lv_obj_send_event(lvglButtons[i], LV_EVENT_CLICKED, NULL);
          }
        }
      }
    }

    previousStates[i] = reading;
  }
}

void btn_event_handler(lv_event_t *e) {
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);  // add (lv_obj_t*) to fix "invalid conversion from 'void*' to 'lv_obj_t*" error
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    Serial.print("Kliknuto na tlačítko: ");

    if (btn == ui_Button1) {
      Serial.println("Button1");
      _ui_state_modify(ui_Container1, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
    } else if (btn == ui_Button2) {
      Serial.println("Button2");
      _ui_state_modify(ui_Container2, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
    } else if (btn == ui_Button3) {
      Serial.println("Button3");
      _ui_state_modify(ui_Container3, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
    } else if (btn == ui_Button4) {
      Serial.println("Button4");
      _ui_state_modify(ui_Container4, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
    } else if (btn == ui_Button5) {
      Serial.println("Button5");
      _ui_state_modify(ui_Container5, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
    }
  }
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

void draw_fft_level_meter_lvgl(lv_obj_t *canvas) {
  static uint8_t peak_y[NUM_BARS] = { 0 };

  lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);

  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  lv_draw_rect_dsc_t dsc_bar;
  lv_draw_rect_dsc_init(&dsc_bar);
  dsc_bar.bg_opa = LV_OPA_COVER;

  lv_draw_rect_dsc_t dsc_peak;
  lv_draw_rect_dsc_init(&dsc_peak);
  dsc_peak.bg_color = lv_color_white();
  dsc_peak.bg_opa = LV_OPA_COVER;

  for (int i = 0; i < NUM_BARS; i++) {
    int fft_idx = i * (FFT_SIZE / 2) / NUM_BARS;
    int magnitude = fft.get(fft_idx);
    int h = (magnitude * CANVAS_HEIGHT) / fft_auto_max;
    if (h > CANVAS_HEIGHT) h = CANVAS_HEIGHT;

    int x = i * BAR_WIDTH;
    int y_start = CANVAS_HEIGHT - h;

    for (int y = 0; y < h; y++) {
      float ratio = (float)(y) / CANVAS_HEIGHT;
      uint8_t r = 0, g = 0;

      if (ratio <= 0.5f) {
        float f = ratio / 0.5f;
        r = (uint8_t)(f * 255);
        g = 255;
      } else if (ratio <= 0.75f) {
        float f = (ratio - 0.5f) / 0.25f;
        r = 255;
        g = (uint8_t)((1.0f - f) * 255);
      } else {
        r = 255;
        g = 0;
      }

      dsc_bar.bg_color = lv_color_make(r, g, 0);

      int y_pos = CANVAS_HEIGHT - y - 1;
      lv_area_t pixel_bar = {
        x, y_pos,
        x + BAR_WIDTH - 2, y_pos
      };
      lv_draw_rect(&layer, &dsc_bar, &pixel_bar);
    }

    // Peak indikátor
    uint8_t new_peak_y = y_start;
    if (peak_y[i] == 0 || new_peak_y < peak_y[i]) {
      peak_y[i] = new_peak_y;
    } else {
      peak_y[i] += 1;
      if (peak_y[i] > CANVAS_HEIGHT - 2)
        peak_y[i] = CANVAS_HEIGHT - 2;
    }

    lv_area_t peak_area = { x, peak_y[i], x + BAR_WIDTH - 2, peak_y[i] + 1 };
    lv_draw_rect(&layer, &dsc_peak, &peak_area);
  }

  lv_canvas_finish_layer(canvas, &layer);
}


void audio_process_i2s(int16_t *outBuff, uint16_t validSamples, uint8_t bitsPerSample, uint8_t channels, bool *continueI2S) {
  if (bitsPerSample != 16 || channels != 2) return;

  static float previous_sample = 0;
  static constexpr float alpha = 0.2f;
  static constexpr float gain = 1.5f;

  for (uint16_t i = 0; i < validSamples * 2; i += 2) {
    float mono = (outBuff[i] + outBuff[i + 1]) * 0.5f;
    float filtered = previous_sample + alpha * (mono - previous_sample);
    previous_sample = filtered;
    int16_t processed = (int16_t)(filtered * gain);

    if (fft_index < FFT_SIZE * 2) {
      fft_buffer[fft_index++] = processed;
    }
  }

  if (fft_index >= FFT_SIZE * 2) {
    fft.exec(fft_buffer);
    fft_index = 0;
    ready_to_fft = true;
  }

  *continueI2S = true;
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

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);

  initButtons();

  Serial.println("Arduino_GFX LVGL_Arduino_v9 example ");
  String LVGL_Arduino = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  // Volume (0-100)
  audio.setVolume(7);

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
  Serial.printf("Connecting to %s ", SECRET_SSID);

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

  // // Mapování LVGL tlačítek na indexy
  lvglButtons[0] = ui_Button1;
  lvglButtons[1] = ui_Button2;
  lvglButtons[2] = ui_Button3;
  lvglButtons[3] = ui_Button4;
  lvglButtons[4] = ui_Button5;

  for (int i = 0; i < buttonCount; i++) {
    Serial.print("lvglButtons[");
    Serial.print(i);
    Serial.print("] = ");

    if (lvglButtons[i] != nullptr) {
      Serial.println((uintptr_t)lvglButtons[i], HEX);  // vypíše adresu v paměti
    } else {
      Serial.println("nullptr");  // tlačítko není přiřazeno
    }
  }

  lv_obj_add_event_cb(ui_Button1, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button2, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button3, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button4, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button5, btn_event_handler, LV_EVENT_ALL, NULL);

  LV_DRAW_BUF_DEFINE_STATIC(canvas_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_RGB565);
  LV_DRAW_BUF_INIT_STATIC(canvas_buf);

  /*Create a canvas and initialize its palette*/
  canvas = lv_canvas_create(lv_screen_active());
  lv_canvas_set_draw_buf(canvas, &canvas_buf);
  lv_obj_set_parent(canvas, ui_CntnrRadio);
  lv_obj_align(canvas, LV_ALIGN_BOTTOM_MID, 0, -5);
  lv_obj_move_background(canvas);
  lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_TRANSP);

  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);
  lv_canvas_finish_layer(canvas, &layer);

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

  if (ready_to_fft) {
    ready_to_fft = false;
    static unsigned long last_update = 0;
    const unsigned long update_interval = 25;  // ms

    if (millis() - last_update >= update_interval) {
      last_update = millis();
      draw_fft_level_meter_lvgl(canvas);
    }
  }
  // set the brightness on LEDC channel 0
  //ledcWriteChannel(LEDC_CHANNEL, brightness);

  lv_task_handler(); /* let the GUI do its work */
  processButtons();
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
}
