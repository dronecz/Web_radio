#include "gui.h"
#include "app_shared.h"
#include "ui_select.h"
#include "EncoderRead.h"
#include "Audio.h"
#include <Arduino_GFX_Library.h>
#include "FFT.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <atomic>
#include <cmath>
#include <mutex>
#include <vector>

extern Audio audio;
extern EncoderRead encoder;
extern Arduino_GFX *gfx;
extern PlayerMode playerMode;
extern int brightness;
extern lv_obj_t *ui_IconWiFi;
extern lv_obj_t *ui_IconWiFi75;
extern lv_obj_t *ui_IconWiFi50;
extern lv_obj_t *ui_IconWiFi25;
extern lv_obj_t *ui_SldrVolume;
extern lv_obj_t *ui_LblStation;
extern lv_obj_t *ui_LblCurPlaying;
extern lv_obj_t *ui_LblInfo;
extern lv_obj_t *ui_LblDate;
extern lv_obj_t *ui_LblMin;
extern lv_obj_t *ui_LblHrs;
extern lv_obj_t *ui_CntnrRadio;
extern lv_obj_t *ui_ScrMusicPlayer;
extern lv_obj_t *ui_ScrNetworkPlayer;
extern lv_obj_t *ui_Container6;
extern lv_obj_t *ui_Button6;
extern lv_obj_t *ui_Button7;
extern lv_obj_t *ui_Button8;
extern lv_obj_t *ui_Button14;
extern lv_obj_t *ui_Button12;
extern lv_obj_t *ui_Button9;
extern lv_obj_t *ui_Button10;
extern lv_obj_t *ui_Button11;
lv_obj_t *ui_ModeMusicInfo = nullptr;
lv_obj_t *ui_ModeNetworkInfo = nullptr;

static uint32_t screenWidth;
static uint32_t screenHeight;
static uint32_t bufSize;
static lv_display_t *disp;
static lv_color_t *disp_draw_buf;

lv_obj_t *canvas = nullptr;
lv_color_t *canvas_buf = nullptr;
lv_timer_t *connecting_timer = nullptr;
uint8_t dotCount = 0;
int modeMenuSelection = 0;
unsigned long lastWifiSignalCheck = 0;

static lv_obj_t *ui_ModeMusicTitle = nullptr;
static lv_obj_t *ui_ModeNetworkTitle = nullptr;
static std::mutex uiTextMutex;
static char pendingStationText[192] = {0};
static char pendingTitleText[512] = {0};
static std::atomic<bool> pendingStationUpdate{false};
static std::atomic<bool> pendingTitleUpdate{false};

// FFT processing state lives in this TU because FFT.h defines static instances.
static int16_t fft_buffer[FFT_SIZE * 2];
static size_t fft_index = 0;
static std::atomic<uint32_t> fft_cb_calls{0};
static std::atomic<bool> fft_raw_active{false};
static uint32_t fft_auto_max = 300;

static inline void fft_push_mono_sample(float mono)
{
  static float previous_sample = 0.0f;
  static constexpr float alpha = 0.2f;

  float filtered = previous_sample + alpha * (mono - previous_sample);
  previous_sample = filtered;

  int32_t processed = (int32_t)filtered;
  if (processed > 32767)
    processed = 32767;
  if (processed < -32768)
    processed = -32768;

  if (fft_index < FFT_SIZE * 2)
    fft_buffer[fft_index++] = (int16_t)processed;
}

static inline void fft_process_from_int32_stereo(int32_t *outBuff, int32_t validSamples)
{
  int32_t maxAbs = 1;
  for (int32_t i = 0; i < validSamples; i++)
  {
    int32_t monoRaw = (outBuff[i * 2] + outBuff[i * 2 + 1]) / 2;
    int32_t a = abs(monoRaw);
    if (a > maxAbs)
      maxAbs = a;
  }

  static float normGain = 1.0f;
  float targetGain = 12000.0f / (float)maxAbs;
  if (targetGain < 0.05f)
    targetGain = 0.05f;
  if (targetGain > 64.0f)
    targetGain = 64.0f;
  normGain = normGain * 0.85f + targetGain * 0.15f;

  for (int32_t i = 0; i < validSamples; i++)
  {
    float mono = (outBuff[i * 2] + outBuff[i * 2 + 1]) * 0.5f;
    mono *= normGain;
    fft_push_mono_sample(mono);
  }

  if (fft_index >= FFT_SIZE * 2)
  {
    fft.exec(fft_buffer);
    fft_index = 0;
  }
}

static void handle_audio_process_raw_samples(int32_t *outBuff, int32_t validSamples)
{
  fft_raw_active.store(true, std::memory_order_relaxed);
  fft_cb_calls.fetch_add(1, std::memory_order_relaxed);
  fft_process_from_int32_stereo(outBuff, validSamples);
}

static void handle_audio_process_i2s(int32_t *outBuff, int32_t validSamples, bool *continueI2S)
{
  if (fft_raw_active.load(std::memory_order_relaxed))
  {
    *continueI2S = true;
    return;
  }

  fft_cb_calls.fetch_add(1, std::memory_order_relaxed);
  fft_process_from_int32_stereo(outBuff, validSamples);
  *continueI2S = true;
}

// Bind exact weak callback symbols from ESP32-audioI2S.
extern "C" void audio_process_raw_samples_bridge(long *outBuff, short validSamples) asm("_Z25audio_process_raw_samplesPls");
extern "C" void audio_process_i2s_bridge(long *outBuff, short validSamples, bool *continueI2S) asm("_Z17audio_process_i2sPlsPb");

extern "C" void audio_process_raw_samples_bridge(long *outBuff, short validSamples)
{
  handle_audio_process_raw_samples(reinterpret_cast<int32_t *>(outBuff), (int32_t)validSamples);
}

extern "C" void audio_process_i2s_bridge(long *outBuff, short validSamples, bool *continueI2S)
{
  handle_audio_process_i2s(reinterpret_cast<int32_t *>(outBuff), (int32_t)validSamples, continueI2S);
}

lv_obj_t *getModeMenuButtonBySelection(int sel)
{
  if (sel == 0)
    return ui_Button6;
  if (sel == 1)
    return ui_Button7;
  return ui_Button8;
}

static int modeToMenuSelection(PlayerMode mode)
{
  switch (mode)
  {
  case MODE_SD_MP3:
    return 1;
  case MODE_JELLYFIN:
    return 2;
  case MODE_RADIO:
  default:
    return 0;
  }
}

void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}

uint32_t millis_cb(void)
{
  return millis();
}

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
#ifndef DIRECT_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif
  lv_disp_flush_ready(disp);
}

void encoder_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  static int32_t last_counter = 0;

  int32_t counter = encoder.getCounter();
  bool btn_state = encoder.encBtn();

  data->enc_diff = counter - last_counter;
  last_counter = counter;
  data->state = btn_state ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void ensureModeScreensUi()
{
  if (ui_ScrMusicPlayer && !ui_ModeMusicTitle)
  {
    ui_ModeMusicTitle = lv_label_create(ui_ScrMusicPlayer);
    lv_obj_set_align(ui_ModeMusicTitle, LV_ALIGN_TOP_MID);
    lv_obj_set_y(ui_ModeMusicTitle, 20);
    lv_obj_set_style_text_font(ui_ModeMusicTitle, &ui_font_Roboto_Reg_18, LV_PART_MAIN);
    lv_label_set_text(ui_ModeMusicTitle, "SD MP3 Player");

    ui_ModeMusicInfo = lv_label_create(ui_ScrMusicPlayer);
    lv_obj_set_width(ui_ModeMusicInfo, lv_pct(90));
    lv_obj_set_align(ui_ModeMusicInfo, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(ui_ModeMusicInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_long_mode(ui_ModeMusicInfo, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_anim_duration(ui_ModeMusicInfo, 5000, LV_PART_MAIN);
    lv_label_set_text(ui_ModeMusicInfo, "No SD track selected");
  }

  if (ui_ScrNetworkPlayer && !ui_ModeNetworkTitle)
  {
    ui_ModeNetworkTitle = lv_label_create(ui_ScrNetworkPlayer);
    lv_obj_set_align(ui_ModeNetworkTitle, LV_ALIGN_TOP_MID);
    lv_obj_set_y(ui_ModeNetworkTitle, 20);
    lv_obj_set_style_text_font(ui_ModeNetworkTitle, &ui_font_Roboto_Reg_18, LV_PART_MAIN);
    lv_label_set_text(ui_ModeNetworkTitle, "Jellyfin Player");

    ui_ModeNetworkInfo = lv_label_create(ui_ScrNetworkPlayer);
    lv_obj_set_width(ui_ModeNetworkInfo, lv_pct(90));
    lv_obj_set_align(ui_ModeNetworkInfo, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(ui_ModeNetworkInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_long_mode(ui_ModeNetworkInfo, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_anim_duration(ui_ModeNetworkInfo, 5000, LV_PART_MAIN);
    lv_label_set_text(ui_ModeNetworkInfo, "No Jellyfin stream configured");
  }
}

void updateModeMenuSelectionVisual()
{
  if (!ui_Button6 || !ui_Button7 || !ui_Button8)
    return;

  lv_obj_t *buttons[3] = {ui_Button6, ui_Button7, ui_Button8};
  for (int i = 0; i < 3; i++)
  {
    if (i == modeMenuSelection)
      lv_obj_add_state(buttons[i], LV_STATE_CHECKED);
    else
      lv_obj_clear_state(buttons[i], LV_STATE_CHECKED);
  }
}

bool isModeMenuVisible()
{
  if (!ui_Container6)
    return false;
  return !lv_obj_has_flag(ui_Container6, LV_OBJ_FLAG_HIDDEN);
}

void hideModeMenu()
{
  if (!ui_Container6)
    return;
  lv_obj_add_flag(ui_Container6, LV_OBJ_FLAG_HIDDEN);
}

void showModeMenu()
{
  if (!ui_Container6)
    return;

  lv_obj_t *active = lv_screen_active();
  if (active)
  {
    lv_obj_set_parent(ui_Container6, active);
    lv_obj_align(ui_Container6, LV_ALIGN_CENTER, 0, -10);
  }
  modeMenuSelection = modeToMenuSelection(playerMode);
  updateModeMenuSelectionVisual();
  lv_obj_clear_flag(ui_Container6, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(ui_Container6);
}

void ensureVolumeSliderVisibleOnActiveScreen()
{
  if (!ui_SldrVolume)
    return;

  lv_obj_t *active = lv_screen_active();
  if (!active)
    return;

  if (lv_obj_get_parent(ui_SldrVolume) != active)
    lv_obj_set_parent(ui_SldrVolume, active);

  lv_obj_align(ui_SldrVolume, LV_ALIGN_TOP_MID, 5, 0);
  lv_obj_move_foreground(ui_SldrVolume);
  updateWiFiSignalIcon();
}

void setModeInfoLabel(lv_obj_t *label, const char *text)
{
  if (!label)
    return;
  if (!text)
    text = "";

  lv_obj_t *parent = lv_obj_get_parent(label);
  lv_coord_t max_w = lv_obj_get_width(label);
  if (parent)
  {
    lv_coord_t parent_w = lv_obj_get_content_width(parent);
    if (parent_w > 8)
      max_w = parent_w - 8;
  }
  if (max_w <= 0)
    max_w = 100;

  lv_obj_set_width(label, max_w);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

  const lv_font_t *font = (const lv_font_t *)lv_obj_get_style_text_font(label, LV_PART_MAIN);
  lv_point_t size;
  lv_text_get_size(&size, text, font, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

  if (size.x > max_w)
  {
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_anim_duration(label, 5000, LV_PART_MAIN);
  }
  else
  {
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
  }

  lv_label_set_text(label, text);
}

void processPendingLabelUpdates()
{
  char stationLocal[192] = {0};
  char titleLocal[512] = {0};
  bool doStation = false;
  bool doTitle = false;

  if (pendingStationUpdate.exchange(false, std::memory_order_acq_rel))
  {
    std::lock_guard<std::mutex> lock(uiTextMutex);
    strncpy(stationLocal, pendingStationText, sizeof(stationLocal) - 1);
    doStation = true;
  }

  if (pendingTitleUpdate.exchange(false, std::memory_order_acq_rel))
  {
    std::lock_guard<std::mutex> lock(uiTextMutex);
    strncpy(titleLocal, pendingTitleText, sizeof(titleLocal) - 1);
    doTitle = true;
  }

  if (doStation)
    setModeInfoLabel(ui_LblStation, stationLocal);
  if (doTitle)
    setModeInfoLabel(ui_LblCurPlaying, titleLocal);
}

void my_audio_info(Audio::msg_t m)
{
  switch (m.e)
  {
  case Audio::evt_name:
    {
      std::lock_guard<std::mutex> lock(uiTextMutex);
      strncpy(pendingStationText, m.msg ? m.msg : "", sizeof(pendingStationText) - 1);
      pendingStationText[sizeof(pendingStationText) - 1] = '\0';
      pendingStationUpdate.store(true, std::memory_order_release);
    }
    break;
  case Audio::evt_streamtitle:
    {
      std::lock_guard<std::mutex> lock(uiTextMutex);
      strncpy(pendingTitleText, m.msg ? m.msg : "", sizeof(pendingTitleText) - 1);
      pendingTitleText[sizeof(pendingTitleText) - 1] = '\0';
      pendingTitleUpdate.store(true, std::memory_order_release);
    }
    break;
  }
}

bool wifiCredentialsStored()
{
  wifi_config_t conf;
  esp_wifi_get_config(WIFI_IF_STA, &conf);
  return strlen((char *)conf.sta.ssid) != 0;
}

void connecting_animation(lv_timer_t *timer)
{
  static char buff[32];
  LV_UNUSED(timer);

  dotCount = (dotCount + 1) % 4;
  snprintf(buff, sizeof(buff), "Connecting%s",
           dotCount == 0 ? "" : dotCount == 1 ? "."
                            : dotCount == 2   ? ".."
                                              : "...");
  lv_label_set_text(ui_LblInfo, buff);
}

static void ensureWiFiIconsVisibleOnActiveScreen()
{
  if (!ui_IconWiFi || !ui_IconWiFi75 || !ui_IconWiFi50 || !ui_IconWiFi25)
    return;

  if (playerMode == MODE_SD_MP3)
  {
    lv_obj_add_flag(ui_IconWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_IconWiFi75, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_IconWiFi50, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_IconWiFi25, LV_OBJ_FLAG_HIDDEN);
    return;
  }

  lv_obj_t *active = lv_screen_active();
  if (!active)
    return;

  lv_obj_t *icons[4] = {ui_IconWiFi, ui_IconWiFi75, ui_IconWiFi50, ui_IconWiFi25};
  for (int i = 0; i < 4; i++)
  {
    if (lv_obj_get_parent(icons[i]) != active)
      lv_obj_set_parent(icons[i], active);

    lv_obj_align(icons[i], LV_ALIGN_TOP_RIGHT, -8, 4);
    lv_obj_move_foreground(icons[i]);
  }
}

void updateWiFiSignalIcon()
{
  const lv_style_selector_t wifiStyleSel = (lv_style_selector_t)LV_PART_MAIN;

  ensureWiFiIconsVisibleOnActiveScreen();

  if (!ui_IconWiFi || !ui_IconWiFi75 || !ui_IconWiFi50 || !ui_IconWiFi25)
    return;

  lv_obj_add_flag(ui_IconWiFi, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_IconWiFi75, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_IconWiFi50, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_IconWiFi25, LV_OBJ_FLAG_HIDDEN);

  if (playerMode == MODE_SD_MP3)
    return;

  if (WiFi.status() != WL_CONNECTED)
  {
    lv_obj_clear_flag(ui_IconWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_image_recolor(ui_IconWiFi, lv_color_hex(0xFF0000), wifiStyleSel);
    lv_obj_set_style_image_recolor_opa(ui_IconWiFi, 255, wifiStyleSel);
    return;
  }

  lv_obj_set_style_image_recolor_opa(ui_IconWiFi, 0, wifiStyleSel);

  int strengthPercent = 2 * (WiFi.RSSI() + 100);
  if (strengthPercent < 0)
    strengthPercent = 0;
  if (strengthPercent > 100)
    strengthPercent = 100;

  if (strengthPercent > 75)
    lv_obj_clear_flag(ui_IconWiFi, LV_OBJ_FLAG_HIDDEN);
  else if (strengthPercent > 50)
    lv_obj_clear_flag(ui_IconWiFi75, LV_OBJ_FLAG_HIDDEN);
  else if (strengthPercent > 25)
    lv_obj_clear_flag(ui_IconWiFi50, LV_OBJ_FLAG_HIDDEN);
  else
    lv_obj_clear_flag(ui_IconWiFi25, LV_OBJ_FLAG_HIDDEN);
}

void displaySetup()
{
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  if (!gfx->begin())
    Serial.println("gfx->begin() failed!");
  gfx->fillScreen(RGB565_BLACK);

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
#else
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
#endif
#else
  disp_draw_buf = (lv_color_t *)malloc(bufSize * 2);
#endif

  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
    return;
  }

  disp = lv_display_create(screenWidth, screenHeight);
  lv_display_set_flush_cb(disp, my_disp_flush);
#ifdef DIRECT_MODE
  lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
  lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
}

void draw_fft_level_meter_lvgl(lv_obj_t *canvasObj)
{
  static float bar_level[35] = {0.0f};
  static uint8_t peak_y[35] = {0};
  static uint8_t peak_hold[35] = {0};
  uint32_t frame_max = 1;
  bool use_vu_fallback = (fft_cb_calls.load(std::memory_order_relaxed) == 0);
  uint16_t vu = audio.getVUlevel();
  uint8_t vu_l = (uint8_t)(vu & 0xFF);
  uint8_t vu_r = (uint8_t)((vu >> 8) & 0xFF);
  bool use_idle_animation = use_vu_fallback && (vu_l == 0) && (vu_r == 0);
  const int numBars = 35;
  const int canvasW = (int)lv_obj_get_width(canvasObj);
  const int canvasH = (int)lv_obj_get_height(canvasObj);
  const int barStride = (canvasW > numBars) ? (canvasW / numBars) : 3;
  const int barWidth = (barStride > 1) ? (barStride - 1) : 1;

  lv_canvas_fill_bg(canvasObj, lv_color_black(), LV_OPA_COVER);

  lv_layer_t layer;
  lv_canvas_init_layer(canvasObj, &layer);

  lv_draw_rect_dsc_t dsc_bar;
  lv_draw_rect_dsc_init(&dsc_bar);
  dsc_bar.bg_opa = LV_OPA_COVER;

  lv_draw_rect_dsc_t dsc_peak;
  lv_draw_rect_dsc_init(&dsc_peak);
  dsc_peak.bg_color = lv_color_make(240, 240, 255);
  dsc_peak.bg_opa = LV_OPA_COVER;

  for (int i = 0; i < numBars; i++)
  {
    uint32_t magnitude = 0;

    if (use_vu_fallback)
    {
      if (use_idle_animation)
      {
        float t = millis() * 0.004f;
        float p = (numBars > 1) ? ((float)i / (float)(numBars - 1)) : 0.0f;
        float w1 = 0.5f + 0.5f * sinf((p * 7.0f) - t);
        float w2 = 0.5f + 0.5f * sinf((p * 11.0f) + t * 1.37f);
        magnitude = (uint32_t)((w1 * 0.7f + w2 * 0.3f) * 180.0f);
      }
      else
      {
        float p = (numBars > 1) ? ((float)i / (float)(numBars - 1)) : 0.0f;
        bool left_half = (p < 0.5f);
        float local = left_half ? (p * 2.0f) : ((p - 0.5f) * 2.0f);
        float shape = 0.65f + 0.35f * (1.0f - fabsf(local - 0.5f) * 2.0f);
        uint8_t base = left_half ? vu_l : vu_r;
        magnitude = (uint32_t)(base * shape);
      }
    }
    else
    {
      int bin_start = (i * i * (FFT_SIZE / 2 - 2)) / (numBars * numBars) + 1;
      int bin_end = (((i + 1) * (i + 1) * (FFT_SIZE / 2 - 2)) / (numBars * numBars)) + 1;
      if (bin_end <= bin_start)
        bin_end = bin_start + 1;

      uint32_t mag_sum = 0;
      uint32_t mag_max = 0;
      int cnt = 0;
      for (int b = bin_start; b < bin_end; b++)
      {
        uint32_t m = fft.get(b);
        mag_sum += m;
        if (m > mag_max)
          mag_max = m;
        cnt++;
      }
      magnitude = cnt > 0 ? ((mag_sum / cnt) + mag_max) / 2 : 0;
    }

    if (magnitude > frame_max)
      frame_max = magnitude;

    uint32_t scale = use_vu_fallback ? 255 : (fft_auto_max > 0 ? fft_auto_max : 1);
    float n = (float)magnitude / (float)scale;
    if (n < 0.03f)
      n = 0.0f;
    if (n > 1.0f)
      n = 1.0f;
    n = sqrtf(n);

    int target = (int)(n * canvasH);
    if (target > (int)bar_level[i])
      bar_level[i] = bar_level[i] * 0.15f + target * 0.85f;
    else
      bar_level[i] = bar_level[i] * 0.55f + target * 0.45f;

    int h = (int)bar_level[i];
    if (h > canvasH - 1)
      h = canvasH - 1;
    if (h < 0)
      h = 0;

    int x = i * barStride;
    int y_start = canvasH - h;

    for (int y = 0; y < h; y++)
    {
      if ((y & 0x03) == 0)
        continue;

      float ratio = (float)y / (float)canvasH;
      uint8_t r = 0, g = 0, b = 0;
      if (ratio <= 0.45f)
      {
        float f = ratio / 0.45f;
        g = (uint8_t)(160 + (95 * f));
        b = (uint8_t)(220 * (1.0f - f));
      }
      else if (ratio <= 0.75f)
      {
        float f = (ratio - 0.45f) / 0.30f;
        r = (uint8_t)(255 * f);
        g = (uint8_t)(255 - (85 * f));
      }
      else
      {
        float f = (ratio - 0.75f) / 0.25f;
        r = 255;
        g = (uint8_t)(170 * (1.0f - f));
      }

      dsc_bar.bg_color = lv_color_make(r, g, b);
      int y_pos = canvasH - y - 1;
      lv_area_t pixel_bar = {x, y_pos, x + barWidth, y_pos};
      lv_draw_rect(&layer, &dsc_bar, &pixel_bar);
    }

    uint8_t new_peak_y = y_start;
    if (peak_y[i] == 0 || new_peak_y < peak_y[i])
    {
      peak_y[i] = (new_peak_y > 2) ? (new_peak_y - 2) : 0;
      peak_hold[i] = 10;
    }
    else
    {
      if (peak_hold[i] > 0)
        peak_hold[i]--;
      else
        peak_y[i] += 1;
      if (peak_y[i] > canvasH - 2)
        peak_y[i] = canvasH - 2;
    }

    lv_area_t peak_area = {x, peak_y[i], x + barWidth, peak_y[i] + 1};
    lv_draw_rect(&layer, &dsc_peak, &peak_area);
  }

  if (!use_vu_fallback && frame_max > fft_auto_max)
    fft_auto_max = frame_max;
  else if (!use_vu_fallback)
    fft_auto_max = (fft_auto_max * 7 + frame_max) / 8;

  if (!use_vu_fallback && fft_auto_max < 40)
    fft_auto_max = 40;

  lv_canvas_finish_layer(canvasObj, &layer);
}

void debug_canvas_heartbeat()
{
#if 0
  static unsigned long lastBeat = 0;
  static bool phase = false;
  unsigned long now = millis();
  if (!canvas)
    return;
  if (now - lastBeat >= 300)
  {
    lastBeat = now;
    phase = !phase;
    lv_color_t c = phase ? lv_color_make(20, 120, 255) : lv_color_make(255, 60, 40);
    lv_canvas_fill_bg(canvas, c, LV_OPA_COVER);
    lv_obj_invalidate(canvas);
  }
#endif
}
