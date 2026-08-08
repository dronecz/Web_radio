#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <WiFiManager.h>
#include <Preferences.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>
#include <vector>
#include <algorithm>
#include <functional>
#include "Audio.h"
#include "EncoderRead.h"
#include "app_shared.h"
#include "gui.h"
#include "inputs.h"
#include "mp3_player.h"
#include "jellyfin_player.h"
#include "ui_select.h"
#include "stations.h"
#include "skin.h"
#include "display_config.h"

static constexpr const char *OTA_HOSTNAME = "web-radio";
static constexpr const char *OTA_PASSWORD = "";

Audio audio;
WiFiManager wm;
Preferences prefs;
EncoderRead encoder(21, 14, 46);

#ifdef DISPLAY_DEV_KIT
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else
#define GFX_BL DISPLAY_BL_PIN
Arduino_DataBus *bus = new Arduino_ESP32SPI(DISPLAY_PIN_DC, DISPLAY_PIN_CS, DISPLAY_PIN_SCK, DISPLAY_PIN_MOSI, DISPLAY_PIN_MISO, FSPI);
#if DISPLAY_DRIVER_ST7789
Arduino_GFX *gfx = new Arduino_ST7789(bus, DISPLAY_PIN_RST, DISPLAY_ROTATION, DISPLAY_ST7789_IS_IPS);
#elif DISPLAY_DRIVER_ST7796
Arduino_GFX *gfx = new Arduino_ST7796(bus, DISPLAY_PIN_RST, DISPLAY_ROTATION);
#elif DISPLAY_DRIVER_ILI9341
Arduino_GFX *gfx = new Arduino_ILI9341(bus, DISPLAY_PIN_RST, DISPLAY_ROTATION);
#endif
#endif

lv_indev_t *indev_encoder;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

int year = 0, month = 0, day = 0, hour = 0, minutes = 0, sec = 0;
static unsigned long targetCountTime = 0;
int brightness = 32;

PlayerMode playerMode = MODE_RADIO;
uint8_t currentStation = 0;
const uint8_t numOfStations = 5;

int buttonCount = 5;
int buttonPins[5] = {40, 41, 42, 44, 43};
bool currentStates[5];
bool previousStates[5];
unsigned long lastDebounceTime[5];
unsigned long debounceDelay = 50;
int activeButton = -1;

int volumePin = 3;
int samples = 10;
unsigned long lastVolumeCheck = 0;
unsigned long volumeCheckInterval = 250;
int changeTreshold = 1;
int lastSliderValue = -1;

lv_obj_t *lvglButtons[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};

void setPlayerMode(PlayerMode mode);
static void saveStation(uint8_t station);
static uint8_t loadStation();
void connectToStation(uint8_t station);
static void connectToWiFi();
static void syncTime();
static void setupOtaUpdate();
static void countTime();
static void printLocalTime();
static void timeavailable(struct timeval *t);

static void logBootStep(const char *msg)
{
  Serial.print("[BOOT] ");
  Serial.println(msg);
}

void setPlayerMode(PlayerMode mode)
{
  PlayerMode previousMode = playerMode;

  if (previousMode == MODE_SD_MP3 && mode != MODE_SD_MP3)
  {
    audio.stopSong();
    sdTrackLoaded = false;
    sdPlaybackPaused = false;
    sdWasRunning = false;
    updateSdPlayPauseVisual();
    updateSdPlayTimeSlider();
  }

  playerMode = mode;
  hideModeMenu();
  ensureModeScreensUi();

  switch (playerMode)
  {
  case MODE_RADIO:
    lv_scr_load(ui_ScrRadioPlayer);
    ensureVolumeSliderVisibleOnActiveScreen();
    if (previousMode != MODE_RADIO)
    {
      lv_obj_t *stationBtn = nullptr;
      switch (currentStation)
      {
      case 0: stationBtn = ui_Button1; break;
      case 1: stationBtn = ui_Button2; break;
      case 2: stationBtn = ui_Button3; break;
      case 3: stationBtn = ui_Button4; break;
      case 4: stationBtn = ui_Button5; break;
      }
      if (stationBtn)
        lv_obj_send_event(stationBtn, LV_EVENT_CLICKED, NULL);
    }
    break;
  case MODE_SD_MP3:
    lv_scr_load(ui_ScrMusicPlayer);
    ensureVolumeSliderVisibleOnActiveScreen();
    updateSdPlayPauseVisual();
    updateSdFolderButtonsState();
    playSdTrack(currentSdTrack);
    break;
  case MODE_JELLYFIN:
    lv_scr_load(ui_ScrNetworkPlayer);
    ensureVolumeSliderVisibleOnActiveScreen();
    playJellyfinTrack(currentJellyfinTrack);
    break;
  }
}

void playNextInCurrentMode()
{
  if (playerMode == MODE_RADIO)
  {
    uint8_t station = (currentStation + 1) % numOfStations;
    switch (station)
    {
    case 0: lv_obj_send_event(ui_Button1, LV_EVENT_CLICKED, NULL); break;
    case 1: lv_obj_send_event(ui_Button2, LV_EVENT_CLICKED, NULL); break;
    case 2: lv_obj_send_event(ui_Button3, LV_EVENT_CLICKED, NULL); break;
    case 3: lv_obj_send_event(ui_Button4, LV_EVENT_CLICKED, NULL); break;
    case 4: lv_obj_send_event(ui_Button5, LV_EVENT_CLICKED, NULL); break;
    }
  }
  else if (playerMode == MODE_SD_MP3)
  {
    playSdTrack(currentSdTrack + 1);
  }
  else
  {
    playJellyfinTrack(currentJellyfinTrack + 1);
  }
}

void playPrevInCurrentMode()
{
  if (playerMode == MODE_RADIO)
  {
    int station = (int)currentStation - 1;
    if (station < 0)
      station = numOfStations - 1;
    switch (station)
    {
    case 0: lv_obj_send_event(ui_Button1, LV_EVENT_CLICKED, NULL); break;
    case 1: lv_obj_send_event(ui_Button2, LV_EVENT_CLICKED, NULL); break;
    case 2: lv_obj_send_event(ui_Button3, LV_EVENT_CLICKED, NULL); break;
    case 3: lv_obj_send_event(ui_Button4, LV_EVENT_CLICKED, NULL); break;
    case 4: lv_obj_send_event(ui_Button5, LV_EVENT_CLICKED, NULL); break;
    }
  }
  else if (playerMode == MODE_SD_MP3)
  {
    playSdTrack(currentSdTrack - 1);
  }
  else
  {
    playJellyfinTrack(currentJellyfinTrack - 1);
  }
}

static void saveStation(uint8_t station)
{
  if (station >= numOfStations)
    return;

  prefs.begin("radio", false);
  prefs.putUChar("station", station);
  prefs.end();
}

static uint8_t loadStation()
{
  prefs.begin("radio", true);
  uint8_t station = prefs.getUChar("station", 0);
  prefs.end();
  if (station >= numOfStations)
    station = 0;
  return station;
}

static bool loadSavedWiFiCredentials(String &ssid, String &password)
{
  prefs.begin("wifi", true);
  ssid = prefs.getString("ssid", "");
  password = prefs.getString("password", "");
  prefs.end();

  return !ssid.isEmpty() && !password.isEmpty();
}

static void saveWiFiCredentials(const String &ssid, const String &password)
{
  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("password", password);
  prefs.end();
}

static bool tryConnectToSavedWiFi(const String &ssid, const String &password)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true, true);
  WiFi.begin(ssid.c_str(), password.c_str());

  uint32_t start = millis();
  const uint32_t timeout = 10000;
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeout)
  {
    lv_timer_handler();
    delay(5);
  }

  return WiFi.status() == WL_CONNECTED;
}

void connectToStation(uint8_t station)
{
  currentStation = station;
  audio.stopSong();
  audio.connecttohost(stations[station]);
  saveStation(station);
}

void connectToWiFi()
{
  logBootStep("WiFi connect start");
  lv_label_set_text(ui_LblInfo, "Connecting");
  connecting_timer = lv_timer_create(connecting_animation, 400, NULL);

  String savedSsid;
  String savedPassword;
  const bool hasSavedCredentials = loadSavedWiFiCredentials(savedSsid, savedPassword);

  if (hasSavedCredentials)
  {
    logBootStep("Using saved WiFi credentials from NVS");
    if (tryConnectToSavedWiFi(savedSsid, savedPassword))
    {
      lv_timer_del(connecting_timer);
      connecting_timer = nullptr;
      Serial.print("[BOOT] WiFi connected, IP: ");
      Serial.println(WiFi.localIP());
      lv_label_set_text(ui_LblInfo, "Connected!");
      delay(600);
      lv_scr_load(ui_ScrRadioPlayer);
      return;
    }

    logBootStep("Saved WiFi credentials failed, launching WiFiManager portal");
  }
  else
  {
    logBootStep("No saved WiFi credentials found, launching WiFiManager portal");
  }

  lv_timer_del(connecting_timer);
  connecting_timer = nullptr;

  lv_label_set_text(ui_LblInfo, "Open setup portal");
  lv_scr_load(ui_ScrWiFiManager);
  createSkinMenuUI();
  delay(800);

  wm.setConfigPortalTimeout(180);
  wm.setHostname("web-radio");
  const bool portalConnected = wm.autoConnect("MusicPlayerAP", "password");

  if (portalConnected && WiFi.status() == WL_CONNECTED)
  {
    saveWiFiCredentials(WiFi.SSID(), WiFi.psk());
    Serial.print("[BOOT] WiFiManager connected, IP: ");
    Serial.println(WiFi.localIP());
    lv_label_set_text(ui_LblInfo, "Connected!");
    delay(600);
    lv_scr_load(ui_ScrRadioPlayer);
  }
  else
  {
    logBootStep("WiFiManager finished without connection");
    lv_label_set_text(ui_LblInfo, "WiFi setup pending");
  }
}

static void setupOtaUpdate()
{
  if (WiFi.status() != WL_CONNECTED)
    return;

  ArduinoOTA.setHostname(OTA_HOSTNAME);
  if (strlen(OTA_PASSWORD) > 0)
    ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.begin();
}

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
    return;
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void timeavailable(struct timeval *t)
{
  LV_UNUSED(t);
  printLocalTime();
}

static void syncTime()
{
  logBootStep("NTP sync start");
  struct tm timeinfo;
  const uint32_t start = millis();
  const uint32_t timeoutMs = 15000;

  while (!getLocalTime(&timeinfo) && (millis() - start) < timeoutMs)
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(500);
  }

  if (!getLocalTime(&timeinfo))
  {
    logBootStep("NTP sync timeout, continuing without RTC time");
    return;
  }

  year = timeinfo.tm_year + 1900;
  month = timeinfo.tm_mon + 1;
  day = timeinfo.tm_mday;
  hour = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  sec = timeinfo.tm_sec;
  targetCountTime = millis() + 1000;
  logBootStep("NTP sync done");
}

static void countTime()
{
  unsigned long currMillisCountTime = millis();
  char numberString[3];

  if (targetCountTime == 0)
  {
    targetCountTime = currMillisCountTime + 1000;
    return;
  }

  if (currMillisCountTime >= targetCountTime)
  {
    targetCountTime += 1000;
    sec++;
    if (sec >= 60)
    {
      sec = 0;
      minutes++;
      if (minutes >= 60)
      {
        minutes = 0;
        hour++;
        if (hour >= 24)
          hour = 0;
        sprintf(numberString, "%02d", hour);
        lv_label_set_text(ui_LblHrs, numberString);
      }
      sprintf(numberString, "%02d", minutes);
      lv_label_set_text(ui_LblMin, numberString);
    }
  }
}

void setup()
{
  Audio::audio_info_callback = my_audio_info;
  Serial.begin(115200);
  delay(300);
  logBootStep("setup start");

  initButtons();
#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  audio.setPinout(5, 6, 4);
  audio.setVolumeSteps(21);
  audio.setVolume(7);

  encoder.begin();
  lv_init();
  lv_tick_set_cb(millis_cb);

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  displaySetup();

  indev_encoder = lv_indev_create();
  lv_indev_set_type(indev_encoder, LV_INDEV_TYPE_ENCODER);
  lv_indev_set_read_cb(indev_encoder, encoder_read);

  ui_init();
  ensureModeScreensUi();
  logBootStep("UI ready");

  // Initialize skin system
  SkinManager &skin_mgr = SkinManager::instance();
  skin_mgr.loadSkinPreference();
  applySkinToUI(skin_mgr.getCurrentSkin());
  logBootStep("Skin loaded and applied");

  connectToWiFi();
  setupOtaUpdate();
  syncTime();

  lvglButtons[0] = ui_Button1;
  lvglButtons[1] = ui_Button2;
  lvglButtons[2] = ui_Button3;
  lvglButtons[3] = ui_Button4;
  lvglButtons[4] = ui_Button5;

  lv_obj_add_event_cb(ui_Button1, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button2, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button3, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button4, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button5, btn_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button6, mode_button_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button7, mode_button_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button8, mode_button_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button9, music_player_button_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button10, music_player_button_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button11, music_player_button_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button12, music_player_button_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button14, music_player_button_event_handler, LV_EVENT_ALL, NULL);

  LV_DRAW_BUF_DEFINE_STATIC(canvas_buf, DISPLAY_CANVAS_WIDTH, DISPLAY_CANVAS_HEIGHT, LV_COLOR_FORMAT_RGB565);
  LV_DRAW_BUF_INIT_STATIC(canvas_buf);
  canvas = lv_canvas_create(lv_screen_active());
  lv_canvas_set_draw_buf(canvas, &canvas_buf);
  lv_obj_set_parent(canvas, ui_CntnrRadio);
  lv_obj_align(canvas, LV_ALIGN_BOTTOM_MID, 0, -5);
  lv_obj_move_foreground(canvas);

  char numberMin[3], numberHrs[3], numberDate[11];
  sprintf(numberDate, "%02d.%02d.%04d", day, month, year);
  lv_label_set_text(ui_LblDate, numberDate);
  sprintf(numberMin, "%02d", minutes);
  lv_label_set_text(ui_LblMin, numberMin);
  sprintf(numberHrs, "%02d", hour);
  lv_label_set_text(ui_LblHrs, numberHrs);

  uint8_t station = loadStation();
  connectToStation(station);
  setPlayerMode(MODE_RADIO);
  updateWiFiSignalIcon();
  logBootStep("setup done");
}

void loop()
{
  audio.loop();
  wm.process();

  if (WiFi.status() == WL_CONNECTED)
    ArduinoOTA.handle();

  processPendingLabelUpdates();

  if ((millis() - lastWifiSignalCheck) >= 5000)
  {
    lastWifiSignalCheck = millis();
    updateWiFiSignalIcon();
  }

  static unsigned long last_update = 0;
  const unsigned long update_interval = 33;
  if (playerMode == MODE_RADIO && millis() - last_update >= update_interval)
  {
    last_update = millis();
    draw_fft_level_meter_lvgl(canvas);
  }

  if (playerMode == MODE_SD_MP3)
  {
    updateSdPlayTimeSlider();
    bool running = audio.isRunning();
    const unsigned long sdAutoplayGraceMs = 1500;
    if (sdTrackLoaded && !sdPlaybackPaused && sdWasRunning && !running && (millis() - sdTrackStartMs) > sdAutoplayGraceMs)
    {
      int nextTrack = currentSdTrack + 1;
      if (nextTrack < (int)sdTracks.size())
        playSdTrack(nextTrack);
      else
      {
        sdTrackLoaded = false;
        sdPlaybackPaused = false;
        updateSdPlayPauseVisual();
        updateSdPlayTimeSlider();
      }
    }
    sdWasRunning = running;
  }
  else
  {
    sdWasRunning = false;
  }

  readVolumeValue();
  lv_task_handler();
  processButtons();
  countTime();
  processEncoder();
}
