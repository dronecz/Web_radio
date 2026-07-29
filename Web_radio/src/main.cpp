
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

 https://randomnerdtutorials.com/esp32-tft-lvgl-weather-station/
 https://www.dovora.com/resources/weather-icons/

 https://github.com/pangcrd/LVGL_Bassic-tutorial/tree/main/BassicButton

 */

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "Audio.h"
#include "EncoderRead.h"
#include <lvgl.h>
#include <WiFiManager.h>
#include <Preferences.h>
#include <mutex>

#ifdef small
#include "ui/small/ui.h"
#endif
#ifdef large
#include "ui/large/ui.h"
#endif

#include <WiFi.h>
#include <ArduinoOTA.h>
#include <SD.h>
#include <SPI.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "time.h"
#include "esp_sntp.h"
#include "esp_wifi.h"
#include "esp_ota_ops.h"
#include "stations.h"
#include <vector>
#include <algorithm>
#include <functional>

EncoderRead encoder(21, 14, 47); // PinA, PinB, Button

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// #define DIRECT_MODE  // Uncomment to enable full frame buffer

// Connections ESP32S3 <-> Amplifier
#define I2S_DOUT 4
#define I2S_BCLK 5
#define I2S_LRC 6
Audio audio;

WiFiManager wm;

Preferences prefs;

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

#define GFX_BL 16 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_ESP32SPI(17 /* DC */, 10 /* CS */, 12 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, FSPI /* spi_num */);

#ifdef small
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ST7789(bus, 18 /* RST */, 3 /* rotation */, true /* IPS */);
#endif

#ifdef large
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ST7796(bus, 18 /* RST */, 1 /* rotation */);
#endif

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

lv_timer_t *connecting_timer;
uint8_t dotCount = 0;

#include "FFT.h"
// --- LVGL canvas ---
#ifdef small
#define CANVAS_WIDTH 285
#define CANVAS_HEIGHT 30
#endif
#ifdef large
#define CANVAS_WIDTH 385
#define CANVAS_HEIGHT 40
#endif

static lv_obj_t *canvas;
static lv_color_t *canvas_buf;

#define NUM_BARS 35
#define BAR_WIDTH (CANVAS_WIDTH / NUM_BARS)
#define SAMPLE_RATE 44100
#define DEBUG_CANVAS_HEARTBEAT 0
#define FFT_DEBUG_LOG 0
#define FFT_IDLE_WAVE_WHEN_SILENT 1
static uint32_t fft_auto_max = 300;
float fft_magnitudes[FFT_SIZE / 2] = {0};

// --- FFT ---
int16_t fft_buffer[FFT_SIZE * 2];
size_t fft_index = 0;
std::atomic<bool> ready_to_fft{false};
std::atomic<uint32_t> fft_cb_calls{0};
std::atomic<uint32_t> fft_exec_calls{0};
std::atomic<uint32_t> fft_raw_calls{0};
std::atomic<uint32_t> fft_i2s_calls{0};
std::atomic<bool> fft_raw_active{false};

std::mutex uiTextMutex;
char pendingStationText[192] = {0};
char pendingTitleText[512] = {0};
std::atomic<bool> pendingStationUpdate{false};
std::atomic<bool> pendingTitleUpdate{false};

static unsigned long targetCountTime;
int year, month, day, hour, minutes, sec = 0;

int brightness = 32; // initial brightness of the screen 0 - 255

enum PlayerMode : uint8_t
{
  MODE_RADIO = 0,
  MODE_SD_MP3 = 1,
  MODE_JELLYFIN = 2
};

PlayerMode playerMode = MODE_RADIO;

const int buttonCount = 5;
const int buttonPins[buttonCount] = {40, 41, 42, 44, 43};

bool currentStates[buttonCount];
bool previousStates[buttonCount];
unsigned long lastDebounceTime[buttonCount];
const unsigned long debounceDelay = 50;

int activeButton = -1;

int volumePin = 3;
const int samples = 10;
unsigned long lastVolumeCheck = 0;
const unsigned long volumeCheckInterval = 250;
const int changeTreshold = 1; // Minimální rozdíl pro aktualizaci
int lastSliderValue = -1;     // Interní proměnné pro sledování stavu

//const int numOfStations = sizeof(stations) / sizeof(stations[0]);
uint8_t currentStation = 0;

lv_indev_t *indev_encoder;

lv_obj_t *lvglButtons[buttonCount];

// Ukazatele na LVGL tlačítka (musí být deklarovány někde jinde)
extern lv_obj_t *ui_Button1;
extern lv_obj_t *ui_Button2;
extern lv_obj_t *ui_Button3;
extern lv_obj_t *ui_Button4;
extern lv_obj_t *ui_Button5;

const uint8_t numOfStations = 5;

std::vector<String> sdTracks;
std::vector<String> sdFolders;
int currentSdTrack = 0;
int currentSdFolder = 0;
bool sdInitialized = false;
bool sdScanned = false;
bool sdSpiInitialized = false;
bool sdTrackLoaded = false;
bool sdPlaybackPaused = false;
bool sdWasRunning = false;
unsigned long sdTrackStartMs = 0;
bool otaInitialized = false;

// Shared SPI bus with display wiring:
// MOSI=11, MISO=13, SCK=12, SD CS=2
static const uint8_t SD_CS_PIN = 2;
static const uint8_t SD_MOSI_PIN = 11;
static const uint8_t SD_MISO_PIN = 13;
static const uint8_t SD_SCK_PIN = 12;

struct JellyfinTrack
{
  String id;
  String name;
  String streamUrl;
};

std::vector<JellyfinTrack> jellyfinTracks;
int currentJellyfinTrack = 0;
bool jellyfinReady = false;
String jellyfinBaseUrl;
String jellyfinToken;
String jellyfinUserId;

#ifndef SECRET_JELLYFIN_SERVER
#define SECRET_JELLYFIN_SERVER ""
#endif
#ifndef SECRET_JELLYFIN_USERNAME
#define SECRET_JELLYFIN_USERNAME ""
#endif
#ifndef SECRET_JELLYFIN_PASSWORD
#define SECRET_JELLYFIN_PASSWORD ""
#endif
#ifndef SECRET_JELLYFIN_API_KEY
#define SECRET_JELLYFIN_API_KEY ""
#endif
#ifndef SECRET_JELLYFIN_USER_ID
#define SECRET_JELLYFIN_USER_ID ""
#endif

static constexpr const char *OTA_HOSTNAME = "web-radio";
static constexpr const char *OTA_PASSWORD = "";

static lv_obj_t *ui_ModeMusicTitle = nullptr;
static lv_obj_t *ui_ModeMusicInfo = nullptr;
static lv_obj_t *ui_ModeNetworkTitle = nullptr;
static lv_obj_t *ui_ModeNetworkInfo = nullptr;

static bool encoderBtnPrevPressed = false;
static bool encoderLongPressHandled = false;
static unsigned long encoderPressStart = 0;
static const unsigned long encoderShortPressMinMs = 40;
static const unsigned long encoderLongPressMs = 700;
static unsigned long lastWifiSignalCheck = 0;
static int modeMenuSelection = 0; // 0: Radio, 1: MP3, 2: Jellyfin
static int32_t encoderLastCounter = 0;

void connectToStation(uint8_t station);

static lv_obj_t *getStationButton(uint8_t station)
{
  switch (station)
  {
  case 0:
    return ui_Button1;
  case 1:
    return ui_Button2;
  case 2:
    return ui_Button3;
  case 3:
    return ui_Button4;
  case 4:
    return ui_Button5;
  default:
    return nullptr;
  }
}

static String getFolderFromPath(const String &path)
{
  int slash = path.lastIndexOf('/');
  if (slash <= 0)
    return String("/");
  return path.substring(0, slash);
}

static void updateSdFolderButtonsState()
{
  if (!ui_Button14 || !ui_Button12)
    return;

  const int folderCount = (int)sdFolders.size();
  const bool hasPrevFolder = (folderCount > 0) && (currentSdFolder > 0);
  const bool hasNextFolder = (folderCount > 0) && (currentSdFolder < (folderCount - 1));

  if (hasPrevFolder)
    lv_obj_clear_state(ui_Button14, LV_STATE_CHECKED);
  else
    lv_obj_add_state(ui_Button14, LV_STATE_CHECKED);

  if (hasNextFolder)
    lv_obj_clear_state(ui_Button12, LV_STATE_CHECKED);
  else
    lv_obj_add_state(ui_Button12, LV_STATE_CHECKED);
}

static void updateSdPlayPauseVisual()
{
  if (!ui_Image1 || !ui_Image2)
    return;

  const bool showPlay = !sdTrackLoaded || sdPlaybackPaused;
  if (showPlay)
  {
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_HIDDEN);
  }
}

static void updateSdPlayTimeSlider()
{
  auto setTimeLabels = [](uint32_t currentSec, uint32_t durationSec, bool showPlaceholder)
  {
    if (showPlaceholder)
    {
      if (ui_LblCurPlayedTime)
        lv_label_set_text(ui_LblCurPlayedTime, "--:--");
      if (ui_LblSongDuration)
        lv_label_set_text(ui_LblSongDuration, "--:--");
      return;
    }

    char curBuf[12];
    char durBuf[12];

    uint32_t curMin = currentSec / 60;
    uint32_t curRem = currentSec % 60;
    uint32_t durMin = durationSec / 60;
    uint32_t durRem = durationSec % 60;

    snprintf(curBuf, sizeof(curBuf), "%02lu:%02lu", (unsigned long)curMin, (unsigned long)curRem);
    snprintf(durBuf, sizeof(durBuf), "%02lu:%02lu", (unsigned long)durMin, (unsigned long)durRem);

    if (ui_LblCurPlayedTime)
      lv_label_set_text(ui_LblCurPlayedTime, curBuf);
    if (ui_LblSongDuration)
      lv_label_set_text(ui_LblSongDuration, durBuf);
  };

  if (!ui_SliderPlayTime)
  {
    setTimeLabels(0, 0, true);
    return;
  }

  if (!sdTrackLoaded)
  {
    lv_slider_set_range(ui_SliderPlayTime, 0, 100);
    lv_slider_set_value(ui_SliderPlayTime, 0, LV_ANIM_OFF);
    setTimeLabels(0, 0, true);
    return;
  }

  uint32_t duration = audio.getAudioFileDuration();
  uint32_t current = audio.getAudioCurrentTime();

  if (duration == 0)
  {
    lv_slider_set_range(ui_SliderPlayTime, 0, 100);
    lv_slider_set_value(ui_SliderPlayTime, 0, LV_ANIM_OFF);
    setTimeLabels(current, 0, false);
    return;
  }

  if (current > duration)
    current = duration;

  lv_slider_set_range(ui_SliderPlayTime, 0, (int32_t)duration);
  lv_slider_set_value(ui_SliderPlayTime, (int32_t)current, LV_ANIM_OFF);
  setTimeLabels(current, duration, false);
}

static void rebuildSdFoldersFromTracks()
{
  sdFolders.clear();
  sdFolders.reserve(sdTracks.size());

  String lastFolder;
  for (const String &path : sdTracks)
  {
    String folder = getFolderFromPath(path);
    if (sdFolders.empty() || folder != lastFolder)
    {
      sdFolders.push_back(folder);
      lastFolder = folder;
    }
  }

  if (currentSdTrack >= 0 && currentSdTrack < (int)sdTracks.size())
  {
    const String curFolder = getFolderFromPath(sdTracks[currentSdTrack]);
    currentSdFolder = 0;
    for (int i = 0; i < (int)sdFolders.size(); i++)
    {
      if (sdFolders[i] == curFolder)
      {
        currentSdFolder = i;
        break;
      }
    }
  }
  else
  {
    currentSdFolder = 0;
  }
}

static int firstTrackIndexInFolder(int folderIndex)
{
  if (folderIndex < 0 || folderIndex >= (int)sdFolders.size())
    return -1;

  const String &folder = sdFolders[folderIndex];
  for (int i = 0; i < (int)sdTracks.size(); i++)
  {
    if (getFolderFromPath(sdTracks[i]) == folder)
      return i;
  }
  return -1;
}

static int findNextSdTrackIndex(int currentIndex)
{
  if (currentIndex < 0 || currentIndex >= (int)sdTracks.size())
    return -1;

  if ((currentIndex + 1) >= (int)sdTracks.size())
    return -1;

  // Tracks are sorted by full path, so this naturally advances inside the folder
  // and then to the first track in the next folder.
  return currentIndex + 1;
}

static inline bool hasAudioExtension(const String &name)
{
  String lower = name;
  lower.toLowerCase();
  return lower.endsWith(".mp3") || lower.endsWith(".aac") || lower.endsWith(".wav") || lower.endsWith(".m4a") || lower.endsWith(".flac");
}

static String jellyfinUrlEncode(const String &value)
{
  String out;
  out.reserve(value.length() * 3);
  for (size_t i = 0; i < value.length(); i++)
  {
    char c = value[i];
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
    {
      out += c;
    }
    else
    {
      static const char *hex = "0123456789ABCDEF";
      out += '%';
      out += hex[(c >> 4) & 0x0F];
      out += hex[c & 0x0F];
    }
  }
  return out;
}

static String jellyfinAuthHeaderValue()
{
  return "MediaBrowser Client=\"WebRadio\", Device=\"ESP32-S3\", DeviceId=\"esp32s3_webradio\", Version=\"1.0.0\"";
}

static String jellyfinNormalizeBaseUrl(const char *base)
{
  String out = base ? String(base) : String();
  out.trim();
  while (out.endsWith("/"))
    out.remove(out.length() - 1);
  return out;
}

static void setModeInfoLabel(lv_obj_t *label, const char *text)
{
  if (!label)
    return;
  lv_label_set_text(label, text ? text : "");
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

static int wifiRssiToPercent(int rssi)
{
  // Typical RSSI usable range for WiFi clients: about -100 dBm (weak) to -50 dBm (strong).
  int percent = 2 * (rssi + 100);
  if (percent < 0)
    percent = 0;
  if (percent > 100)
    percent = 100;
  return percent;
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

    // Keep icon at top-right on every screen.
    lv_obj_align(icons[i], LV_ALIGN_TOP_RIGHT, -8, 4);
    lv_obj_move_foreground(icons[i]);
  }
}

static void updateWiFiSignalIcon()
{
  const lv_style_selector_t wifiStyleSel = (lv_style_selector_t)LV_PART_MAIN;

  ensureWiFiIconsVisibleOnActiveScreen();

  if (!ui_IconWiFi || !ui_IconWiFi75 || !ui_IconWiFi50 || !ui_IconWiFi25)
    return;

  lv_obj_add_flag(ui_IconWiFi, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_IconWiFi75, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_IconWiFi50, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_IconWiFi25, LV_OBJ_FLAG_HIDDEN);

  // Show WiFi icon only in modes 0 (radio) and 2 (jellyfin).
  if (playerMode == MODE_SD_MP3)
  {
    return;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    lv_obj_clear_flag(ui_IconWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_image_recolor(ui_IconWiFi, lv_color_hex(0xFF0000), wifiStyleSel);
    lv_obj_set_style_image_recolor_opa(ui_IconWiFi, 255, wifiStyleSel);
    return;
  }

  // Connected: use normal icon colors (no recolor on full icon).
  lv_obj_set_style_image_recolor_opa(ui_IconWiFi, 0, wifiStyleSel);

  int strengthPercent = wifiRssiToPercent(WiFi.RSSI());
  if (strengthPercent > 75)
  {
    lv_obj_clear_flag(ui_IconWiFi, LV_OBJ_FLAG_HIDDEN);
  }
  else if (strengthPercent > 50)
  {
    lv_obj_clear_flag(ui_IconWiFi75, LV_OBJ_FLAG_HIDDEN);
  }
  else if (strengthPercent > 25)
  {
    lv_obj_clear_flag(ui_IconWiFi50, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_obj_clear_flag(ui_IconWiFi25, LV_OBJ_FLAG_HIDDEN);
  }
}

static lv_obj_t *getModeMenuButtonBySelection(int sel)
{
  if (sel == 0)
    return ui_Button6;
  if (sel == 1)
    return ui_Button7;
  return ui_Button8;
}

static void updateModeMenuSelectionVisual()
{
  if (!ui_Button6 || !ui_Button7 || !ui_Button8)
    return;

  lv_obj_t *buttons[3] = {ui_Button6, ui_Button7, ui_Button8};
  for (int i = 0; i < 3; i++)
  {
    if (i == modeMenuSelection)
    {
      lv_obj_add_state(buttons[i], LV_STATE_CHECKED);
    }
    else
    {
      lv_obj_clear_state(buttons[i], LV_STATE_CHECKED);
    }
  }
}

static bool isModeMenuVisible()
{
  if (!ui_Container6)
    return false;
  return !lv_obj_has_flag(ui_Container6, LV_OBJ_FLAG_HIDDEN);
}

static void hideModeMenu()
{
  if (!ui_Container6)
    return;
  lv_obj_add_flag(ui_Container6, LV_OBJ_FLAG_HIDDEN);
}

static void showModeMenu()
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

static void ensureVolumeSliderVisibleOnActiveScreen()
{
  if (!ui_SldrVolume)
    return;

  lv_obj_t *active = lv_screen_active();
  if (!active)
    return;

  if (lv_obj_get_parent(ui_SldrVolume) != active)
  {
    lv_obj_set_parent(ui_SldrVolume, active);
  }

  // Keep generated placement while ensuring it is rendered above screen widgets.
  lv_obj_align(ui_SldrVolume, LV_ALIGN_TOP_MID, 5, 0);
  lv_obj_move_foreground(ui_SldrVolume);

  // WiFi signal should stay visible even while volume bar is active.
  ensureWiFiIconsVisibleOnActiveScreen();
}

static bool jellyfinHttpGet(const String &url, String &response, int &statusCode, bool addToken)
{
  HTTPClient http;
  statusCode = -1;

  if (url.startsWith("https://"))
  {
    WiFiClientSecure client;
    client.setInsecure();
    if (!http.begin(client, url))
      return false;
  }
  else
  {
    WiFiClient client;
    if (!http.begin(client, url))
      return false;
  }

  http.addHeader("Accept", "application/json");
  http.addHeader("X-Emby-Authorization", jellyfinAuthHeaderValue());
  if (addToken && jellyfinToken.length() > 0)
    http.addHeader("X-Emby-Token", jellyfinToken);

  statusCode = http.GET();
  response = http.getString();
  http.end();
  return statusCode > 0;
}

static bool jellyfinHttpPostJson(const String &url, const String &body, String &response, int &statusCode)
{
  HTTPClient http;
  statusCode = -1;

  if (url.startsWith("https://"))
  {
    WiFiClientSecure client;
    client.setInsecure();
    if (!http.begin(client, url))
      return false;
  }
  else
  {
    WiFiClient client;
    if (!http.begin(client, url))
      return false;
  }

  http.addHeader("Accept", "application/json");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Emby-Authorization", jellyfinAuthHeaderValue());

  statusCode = http.POST((uint8_t *)body.c_str(), body.length());
  response = http.getString();
  http.end();
  return statusCode > 0;
}

static bool jellyfinResolveUserIdFromToken()
{
  if (jellyfinUserId.length() > 0)
    return true;

  String payload;
  int code = -1;
  if (!jellyfinHttpGet(jellyfinBaseUrl + "/Users/Me", payload, code, true) || (code < 200 || code >= 300))
    return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err)
    return false;

  const char *id = doc["Id"];
  if (!id || strlen(id) == 0)
    return false;

  jellyfinUserId = id;
  return true;
}

static bool jellyfinLogin()
{
  jellyfinReady = false;
  jellyfinTracks.clear();

  jellyfinBaseUrl = jellyfinNormalizeBaseUrl(SECRET_JELLYFIN_SERVER);
  jellyfinUserId = String(SECRET_JELLYFIN_USER_ID);
  jellyfinToken = "";

  if (jellyfinBaseUrl.length() == 0)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Set SECRET_JELLYFIN_SERVER");
    return false;
  }

  const String apiKey = String(SECRET_JELLYFIN_API_KEY);
  if (apiKey.length() > 0)
  {
    jellyfinToken = apiKey;
    if (!jellyfinResolveUserIdFromToken() && jellyfinUserId.length() == 0)
    {
      setModeInfoLabel(ui_ModeNetworkInfo, "Set SECRET_JELLYFIN_USER_ID for API key mode");
      return false;
    }
    return true;
  }

  const String username = String(SECRET_JELLYFIN_USERNAME);
  const String password = String(SECRET_JELLYFIN_PASSWORD);
  if (username.length() == 0 || password.length() == 0)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Set Jellyfin user/password or API key");
    return false;
  }

  JsonDocument bodyDoc;
  bodyDoc["Username"] = username;
  bodyDoc["Pw"] = password;
  String body;
  serializeJson(bodyDoc, body);

  String payload;
  int code = -1;
  if (!jellyfinHttpPostJson(jellyfinBaseUrl + "/Users/AuthenticateByName", body, payload, code) || (code < 200 || code >= 300))
  {
    Serial.printf("Jellyfin login failed: %d\n", code);
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin login failed");
    return false;
  }

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin auth parse error");
    return false;
  }

  const char *token = doc["AccessToken"];
  const char *uid = doc["User"]["Id"];
  if (!token || !uid)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin auth missing token/user");
    return false;
  }

  jellyfinToken = token;
  jellyfinUserId = uid;
  return true;
}

static bool jellyfinLoadLibrary()
{
  if (jellyfinToken.length() == 0 || jellyfinBaseUrl.length() == 0)
    return false;

  jellyfinTracks.clear();
  const int pageSize = 100;
  int startIndex = 0;
  const int maxItems = 600;

  while ((int)jellyfinTracks.size() < maxItems)
  {
    String url;
    if (jellyfinUserId.length() > 0)
      url = jellyfinBaseUrl + "/Users/" + jellyfinUserId + "/Items";
    else
      url = jellyfinBaseUrl + "/Items";

    url += "?Recursive=true&IncludeItemTypes=Audio&SortBy=SortName&SortOrder=Ascending";
    url += "&StartIndex=" + String(startIndex) + "&Limit=" + String(pageSize);
    url += "&Fields=MediaSources";

    String payload;
    int code = -1;
    if (!jellyfinHttpGet(url, payload, code, true) || (code < 200 || code >= 300))
    {
      Serial.printf("Jellyfin browse failed: %d\n", code);
      return false;
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
      return false;

    JsonArray items = doc["Items"].as<JsonArray>();
    if (items.isNull() || items.size() == 0)
      break;

    for (JsonVariant item : items)
    {
      const char *id = item["Id"];
      const char *name = item["Name"];
      if (!id || !name)
        continue;

      JellyfinTrack track;
      track.id = id;
      track.name = name;
      track.streamUrl = jellyfinBaseUrl + "/Audio/" + track.id + "/stream?static=true&api_key=" + jellyfinUrlEncode(jellyfinToken);
      if (jellyfinUserId.length() > 0)
        track.streamUrl += "&UserId=" + jellyfinUrlEncode(jellyfinUserId);
      jellyfinTracks.push_back(track);

      if ((int)jellyfinTracks.size() >= maxItems)
        break;
    }

    if ((int)items.size() < pageSize)
      break;

    startIndex += pageSize;
  }

  std::sort(jellyfinTracks.begin(), jellyfinTracks.end(), [](const JellyfinTrack &a, const JellyfinTrack &b)
            { return a.name < b.name; });

  return !jellyfinTracks.empty();
}

static bool ensureJellyfinSessionAndLibrary()
{
  if (jellyfinReady && !jellyfinTracks.empty())
    return true;

  setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin: login...");
  if (!jellyfinLogin())
    return false;

  setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin: loading library...");
  if (!jellyfinLoadLibrary())
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin: no tracks or browse failed");
    return false;
  }

  jellyfinReady = true;
  Serial.printf("Jellyfin tracks loaded: %d\n", (int)jellyfinTracks.size());
  return true;
}

static void ensureModeScreensUi()
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

static bool initAndScanSD()
{
  if (!sdInitialized)
  {
    if (!sdSpiInitialized)
    {
      SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
      sdSpiInitialized = true;
      Serial.printf("SD SPI init: CS=%u MOSI=%u MISO=%u SCK=%u\n",
                    (unsigned)SD_CS_PIN,
                    (unsigned)SD_MOSI_PIN,
                    (unsigned)SD_MISO_PIN,
                    (unsigned)SD_SCK_PIN);
    }

    // 16 MHz is usually stable with shared SPI devices and jumper wires.
    sdInitialized = SD.begin(SD_CS_PIN, SPI, 16000000);
    if (!sdInitialized)
    {
      Serial.println("SD init failed");
      setModeInfoLabel(ui_ModeMusicInfo, "SD init failed");
      return false;
    }
  }

  if (sdScanned && !sdTracks.empty())
    return true;

  sdTracks.clear();
  sdFolders.clear();
  sdScanned = true;

  const int maxDepth = 8;
  const int maxTracks = 1500;

  std::function<void(const String &, int)> scanDir = [&](const String &path, int depth)
  {
    if (depth > maxDepth || (int)sdTracks.size() >= maxTracks)
      return;

    File dir = SD.open(path.c_str());
    if (!dir || !dir.isDirectory())
      return;

    File entry = dir.openNextFile();
    while (entry)
    {
      String entryPath = String(entry.path());
      if (entryPath.length() == 0)
      {
        entryPath = String(path);
        if (!entryPath.endsWith("/"))
          entryPath += "/";
        entryPath += String(entry.name());
      }

      if (entry.isDirectory())
      {
        scanDir(entryPath, depth + 1);
      }
      else
      {
        if (hasAudioExtension(entryPath))
          sdTracks.push_back(entryPath);
      }

      if ((int)sdTracks.size() >= maxTracks)
        break;

      entry = dir.openNextFile();
    }
  };

  scanDir("/", 0);

  std::sort(sdTracks.begin(), sdTracks.end());
  rebuildSdFoldersFromTracks();

  Serial.printf("SD tracks found (recursive): %d\n", (int)sdTracks.size());
  if (sdTracks.empty())
  {
    sdTrackLoaded = false;
    sdPlaybackPaused = false;
    updateSdPlayPauseVisual();
    updateSdFolderButtonsState();
    setModeInfoLabel(ui_ModeMusicInfo, "No audio files found on SD");
    return false;
  }

  updateSdFolderButtonsState();
  return true;
}

static void playSdTrack(int index)
{
  ensureModeScreensUi();
  if (!initAndScanSD())
    return;

  if (index < 0)
    index = (int)sdTracks.size() - 1;
  if (index >= (int)sdTracks.size())
    index = 0;
  currentSdTrack = index;

  const String &path = sdTracks[currentSdTrack];

  const String folder = getFolderFromPath(path);
  for (int i = 0; i < (int)sdFolders.size(); i++)
  {
    if (sdFolders[i] == folder)
    {
      currentSdFolder = i;
      break;
    }
  }

  Serial.println("Playing SD: " + path);
  audio.stopSong();
  audio.connecttoFS(SD, path.c_str());
  sdTrackLoaded = true;
  sdPlaybackPaused = false;
  sdTrackStartMs = millis();
  sdWasRunning = false;
  updateSdPlayPauseVisual();
  updateSdPlayTimeSlider();
  updateSdFolderButtonsState();
  setModeInfoLabel(ui_ModeMusicInfo, path.c_str());
}

static bool jumpSdFolder(int direction)
{
  ensureModeScreensUi();
  if (!initAndScanSD())
    return false;

  if (sdFolders.empty())
  {
    updateSdFolderButtonsState();
    return false;
  }

  int targetFolder = currentSdFolder + direction;
  if (targetFolder < 0 || targetFolder >= (int)sdFolders.size())
  {
    updateSdFolderButtonsState();
    return false;
  }

  int trackIndex = firstTrackIndexInFolder(targetFolder);
  if (trackIndex < 0)
  {
    updateSdFolderButtonsState();
    return false;
  }

  currentSdFolder = targetFolder;
  currentSdTrack = trackIndex;
  updateSdFolderButtonsState();

  if (ui_ModeMusicInfo)
  {
    String selected = String("Selected: ") + sdTracks[currentSdTrack];
    setModeInfoLabel(ui_ModeMusicInfo, selected.c_str());
  }

  if (!sdTrackLoaded)
    updateSdPlayTimeSlider();

  return true;
}

void music_player_button_event_handler(lv_event_t *e)
{
  if (lv_event_get_code(e) != LV_EVENT_CLICKED)
    return;

  if (playerMode != MODE_SD_MP3)
    return;

  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);

  if (btn == ui_Button9)
  {
    if (sdTrackLoaded)
      playSdTrack(currentSdTrack - 1);
    return;
  }

  if (btn == ui_Button10)
  {
    ensureModeScreensUi();
    if (!initAndScanSD())
      return;

    if (!sdTrackLoaded)
    {
      playSdTrack(currentSdTrack);
      return;
    }

    audio.pauseResume();
    sdPlaybackPaused = !sdPlaybackPaused;
    updateSdPlayPauseVisual();
    return;
  }

  if (btn == ui_Button11)
  {
    if (sdTrackLoaded)
      playSdTrack(currentSdTrack + 1);
    return;
  }

  if (btn == ui_Button12)
  {
    if (!jumpSdFolder(+1))
      lv_obj_add_state(ui_Button12, LV_STATE_CHECKED);
    return;
  }

  if (btn == ui_Button14)
  {
    if (!jumpSdFolder(-1))
      lv_obj_add_state(ui_Button14, LV_STATE_CHECKED);
    return;
  }
}

static void playJellyfinTrack(int index)
{
  ensureModeScreensUi();
  if (!ensureJellyfinSessionAndLibrary())
    return;

  int total = (int)jellyfinTracks.size();
  if (total <= 0)
  {
    setModeInfoLabel(ui_ModeNetworkInfo, "Jellyfin library is empty");
    return;
  }

  if (index < 0)
    index = total - 1;
  if (index >= total)
    index = 0;

  currentJellyfinTrack = index;
  const JellyfinTrack &track = jellyfinTracks[currentJellyfinTrack];

  Serial.printf("Playing Jellyfin track: %s\n", track.name.c_str());
  audio.stopSong();
  audio.connecttohost(track.streamUrl.c_str());
  setModeInfoLabel(ui_ModeNetworkInfo, track.name.c_str());
}

static void setPlayerMode(PlayerMode mode)
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
  modeMenuSelection = modeToMenuSelection(playerMode);
  updateModeMenuSelectionVisual();
  hideModeMenu();
  ensureModeScreensUi();

  switch (playerMode)
  {
  case MODE_RADIO:
    lv_scr_load(ui_ScrRadioPlayer);
    ensureVolumeSliderVisibleOnActiveScreen();
    if (previousMode != MODE_RADIO)
    {
      lv_obj_t *stationBtn = getStationButton(currentStation);
      if (stationBtn)
      {
        lv_obj_send_event(stationBtn, LV_EVENT_CLICKED, NULL);
      }
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

static void playNextInCurrentMode()
{
  if (playerMode == MODE_RADIO)
  {
    uint8_t station = (currentStation + 1) % numOfStations;
    lv_obj_t *btn = getStationButton(station);
    if (btn)
      lv_obj_send_event(btn, LV_EVENT_CLICKED, NULL);
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

static void playPrevInCurrentMode()
{
  if (playerMode == MODE_RADIO)
  {
    int station = (int)currentStation - 1;
    if (station < 0)
      station = numOfStations - 1;
    lv_obj_t *btn = getStationButton((uint8_t)station);
    if (btn)
      lv_obj_send_event(btn, LV_EVENT_CLICKED, NULL);
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


#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

uint32_t millis_cb(void)
{
  return millis();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
#ifndef DIRECT_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif // #ifndef DIRECT_MODE

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

void encoder_read(lv_indev_t *indev, lv_indev_data_t *data)
{

  static int32_t last_counter = 0;

  int32_t counter = encoder.getCounter();
  bool btn_state = encoder.encBtn();

  data->enc_diff = counter - last_counter;
  last_counter = counter;

  if (btn_state)
    data->state = LV_INDEV_STATE_PRESSED;
  else
    data->state = LV_INDEV_STATE_RELEASED;
}

void initButtons()
{
  for (int i = 0; i < buttonCount; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    currentStates[i] = digitalRead(buttonPins[i]);
    previousStates[i] = currentStates[i];
    lastDebounceTime[i] = 0;
  }
}

void processButtons()
{
  unsigned long currentTime = millis();

  for (int i = 0; i < buttonCount; i++)
  {
    int reading = digitalRead(buttonPins[i]);

    if (reading != previousStates[i])
    {
      lastDebounceTime[i] = currentTime;
    }

    if ((currentTime - lastDebounceTime[i]) > debounceDelay)
    {
      if (reading != currentStates[i])
      {
        currentStates[i] = reading;

        if (currentStates[i] == LOW)
        {
          activeButton = i;

          if (playerMode == MODE_SD_MP3)
          {
            lv_obj_t *musicButtons[buttonCount] = {ui_Button14, ui_Button9, ui_Button10, ui_Button11, ui_Button12};
            lv_obj_t *target = musicButtons[i];
            if (target)
              lv_obj_send_event(target, LV_EVENT_CLICKED, NULL);

            previousStates[i] = reading;
            continue;
          }

          if (playerMode != MODE_RADIO)
          {
            if (i == 0)
            {
              playPrevInCurrentMode();
            }
            else if (i == 1)
            {
              playNextInCurrentMode();
            }
            else if (i == 4)
            {
              setPlayerMode(MODE_RADIO);
            }
            previousStates[i] = reading;
            continue;
          }

          // remove "checked" state from all buttons
          for (int j = 0; j < buttonCount; j++)
          {
            // find ui_Container[n] as children of ui_Buttons[n], "0" means first child of the object
            lv_obj_t *container = lv_obj_get_child(lvglButtons[j], 0);
            _ui_state_modify(container, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
          }
          // check if lvglButtons[i] is not empty
          if (lvglButtons[i] != nullptr)
          {
            // send "click" event to the right button
            lv_obj_send_event(lvglButtons[i], LV_EVENT_CLICKED, NULL);
          }
        }
      }
    }

    previousStates[i] = reading;
  }
}

void processEncoder()
{
  int32_t counter = encoder.getCounter();
  int32_t diff = counter - encoderLastCounter;
  if (diff != 0)
  {
    encoderLastCounter = counter;
    if (isModeMenuVisible())
    {
      if (diff > 0)
      {
        modeMenuSelection = (modeMenuSelection + 1) % 3;
      }
      else
      {
        modeMenuSelection = (modeMenuSelection + 2) % 3;
      }
      updateModeMenuSelectionVisual();
    }
  }

  bool pressed = encoder.encBtn();
  unsigned long now = millis();

  if (pressed && !encoderBtnPrevPressed)
  {
    encoderPressStart = now;
    encoderLongPressHandled = false;
  }

  if (pressed && !encoderLongPressHandled)
  {
    if ((now - encoderPressStart) >= encoderLongPressMs)
    {
      Serial.printf("[ENC] LONG press (%lu ms)\n", (unsigned long)(now - encoderPressStart));
      showModeMenu();
      encoderLongPressHandled = true;
    }
  }

  if (!pressed && encoderBtnPrevPressed)
  {
    unsigned long held = now - encoderPressStart;
    if (!encoderLongPressHandled && held >= encoderShortPressMinMs)
    {
      Serial.printf("[ENC] SHORT press (%lu ms)\n", (unsigned long)held);
      if (isModeMenuVisible())
      {
        lv_obj_t *btn = getModeMenuButtonBySelection(modeMenuSelection);
        if (btn)
          lv_obj_send_event(btn, LV_EVENT_CLICKED, NULL);
      }
    }
  }

  encoderBtnPrevPressed = pressed;
}

void saveStation(uint8_t station)
{
    if (station >= numOfStations) return;

    prefs.begin("radio", false);        // RW mode
    prefs.putUChar("station", station);
    prefs.end();

    Serial.printf("Station %d saved\n", station);
}

uint8_t loadStation()
{
    prefs.begin("radio", true);
    uint8_t station = prefs.getUChar("station", 0);
    prefs.end();
    
    if (station >= numOfStations) {
        Serial.println("Saved station invalid, using 0");
        station = 0;
    }
   Serial.printf("Loaded station %d\n", station);
    return station;
}


void connectToStation(uint8_t station)
{
  currentStation = station;
  audio.stopSong();
  audio.connecttohost(stations[station]);
  saveStation(station);
  Serial.println("Connected to: " + String(stations[station]));
}

void btn_event_handler(lv_event_t *e)
{
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e); // add (lv_obj_t*) to fix "invalid conversion from 'void*' to 'lv_obj_t*" error
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED)
  {
    Serial.print("Kliknuto na tlačítko: ");

    if (btn == ui_Button1)
    {
      Serial.println("Button1");
      _ui_state_modify(ui_Container1, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);

      connectToStation(0);
    }
    else if (btn == ui_Button2)
    {
      Serial.println("Button2");
      _ui_state_modify(ui_Container2, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(1);
    }
    else if (btn == ui_Button3)
    {
      Serial.println("Button3");
      _ui_state_modify(ui_Container3, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(2);
    }
    else if (btn == ui_Button4)
    {
      Serial.println("Button4");
      _ui_state_modify(ui_Container4, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(3);
    }
    else if (btn == ui_Button5)
    {
      Serial.println("Button5");
      _ui_state_modify(ui_Container5, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
      connectToStation(4);
    }
  }
}

void mode_button_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code != LV_EVENT_CLICKED)
    return;

  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  if (btn == ui_Button6)
  {
    setPlayerMode(MODE_RADIO);
  }
  else if (btn == ui_Button7)
  {
    setPlayerMode(MODE_SD_MP3);
  }
  else if (btn == ui_Button8)
  {
    setPlayerMode(MODE_JELLYFIN);
  }
}

int read_potentiometer()
{
  int sum = 0;
  for (int i = 0; i < samples; i++)
  {
    sum += analogRead(volumePin);
    delay(2);
  }
  return sum / 10;
}

void readVolumeValue()
{
  unsigned long now = millis();
  if (now - lastVolumeCheck < volumeCheckInterval)
    return;

  lastVolumeCheck = now;

  int raw = read_potentiometer();
  uint8_t maxVolume = audio.getVolumeSteps();
  int volume = map(raw, 0, 4095, 0, maxVolume);

  if (abs(volume - lastSliderValue) >= changeTreshold)
  {
    ensureVolumeSliderVisibleOnActiveScreen();
    lv_obj_set_style_opa(ui_SldrVolume, LV_OPA_100, 0);
    lv_slider_set_value(ui_SldrVolume, volume, LV_ANIM_OFF);
    lastSliderValue = volume;
    audio.setVolume(volume);
    lv_obj_fade_out(ui_SldrVolume, 1000, 1000);
  }
}

void countTime()
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
    sec++; // Advance second
    if (sec >= 60)
    {
      sec = 0;
      minutes++; // Advance minutes
      if (minutes >= 60)
      {
        minutes = 0;
        hour++; // Advance hour
        if (hour >= 24)
        {
          hour = 0;
        }
        sprintf(numberString, "%02d", hour);
        lv_label_set_text(ui_LblHrs, numberString);
      }

      sprintf(numberString, "%02d", minutes);
      lv_label_set_text(ui_LblMin, numberString);
    }
    // Serial.println("Time is " + String(hh) + (":") + String(mm) + (":") + String(ss));
  }
}

void draw_fft_level_meter_lvgl(lv_obj_t *canvas)
{
  static float bar_level[NUM_BARS] = {0.0f};
  static uint8_t peak_y[NUM_BARS] = {0};
  static uint8_t peak_hold[NUM_BARS] = {0};
  uint32_t frame_max = 1;
  bool use_vu_fallback = (fft_cb_calls.load(std::memory_order_relaxed) == 0);
  uint16_t vu = audio.getVUlevel();
  uint8_t vu_l = (uint8_t)(vu & 0xFF);
  uint8_t vu_r = (uint8_t)((vu >> 8) & 0xFF);
#if FFT_IDLE_WAVE_WHEN_SILENT
  bool use_idle_animation = use_vu_fallback && (vu_l == 0) && (vu_r == 0);
#else
  bool use_idle_animation = false;
#endif

  lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);

  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  lv_draw_rect_dsc_t dsc_bar;
  lv_draw_rect_dsc_init(&dsc_bar);
  dsc_bar.bg_opa = LV_OPA_COVER;

  lv_draw_rect_dsc_t dsc_peak;
  lv_draw_rect_dsc_init(&dsc_peak);
  dsc_peak.bg_color = lv_color_make(240, 240, 255);
  dsc_peak.bg_opa = LV_OPA_COVER;

  for (int i = 0; i < NUM_BARS; i++)
  {
    uint32_t magnitude = 0;

    if (use_vu_fallback)
    {
      if (use_idle_animation)
      {
        // If VU is unavailable too, show a slow idle wave so user still gets visual feedback.
        float t = millis() * 0.004f;
        float p = (NUM_BARS > 1) ? ((float)i / (float)(NUM_BARS - 1)) : 0.0f;
        float w1 = 0.5f + 0.5f * sinf((p * 7.0f) - t);
        float w2 = 0.5f + 0.5f * sinf((p * 11.0f) + t * 1.37f);
        magnitude = (uint32_t)((w1 * 0.7f + w2 * 0.3f) * 180.0f);
      }
      else
      {
        // Fallback mode: create bar graph from L/R VU when FFT hooks are unavailable.
        float p = (NUM_BARS > 1) ? ((float)i / (float)(NUM_BARS - 1)) : 0.0f;
        bool left_half = (p < 0.5f);
        float local = left_half ? (p * 2.0f) : ((p - 0.5f) * 2.0f); // 0..1 within half
        float shape = 0.65f + 0.35f * (1.0f - fabsf(local - 0.5f) * 2.0f); // center emphasis per half
        uint8_t base = left_half ? vu_l : vu_r;
        magnitude = (uint32_t)(base * shape);
      }
    }
    else
    {
      // Log-like bin grouping (better low-frequency visibility than pure linear mapping)
      int bin_start = (i * i * (FFT_SIZE / 2 - 2)) / (NUM_BARS * NUM_BARS) + 1; // skip DC
      int bin_end = (((i + 1) * (i + 1) * (FFT_SIZE / 2 - 2)) / (NUM_BARS * NUM_BARS)) + 1;
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

    // Noise gate + gamma for nicer motion
    if (n < 0.03f)
      n = 0.0f;
    if (n > 1.0f)
      n = 1.0f;
    n = sqrtf(n);

    float target = n * CANVAS_HEIGHT;
    if (target > bar_level[i])
      bar_level[i] = bar_level[i] * 0.15f + target * 0.85f; // very fast attack
    else
      bar_level[i] = bar_level[i] * 0.55f + target * 0.45f; // faster release

    int h = (int)bar_level[i];
    if (h > CANVAS_HEIGHT - 1)
      h = CANVAS_HEIGHT - 1;
    if (h < 0)
      h = 0;

    int x = i * BAR_WIDTH;
    int y_start = CANVAS_HEIGHT - h;

    // Segmented vertical bars (3 px segment, 1 px gap)
    for (int y = 0; y < h; y++)
    {
      if ((y & 0x03) == 0)
        continue;

      float ratio = (float)y / (float)CANVAS_HEIGHT;
      uint8_t r = 0, g = 0;
      uint8_t b = 0;

      if (ratio <= 0.45f)
      {
        // cyan -> green
        float f = ratio / 0.45f;
        r = 0;
        g = (uint8_t)(160 + (95 * f));
        b = (uint8_t)(220 * (1.0f - f));
      }
      else if (ratio <= 0.75f)
      {
        // green -> orange
        float f = (ratio - 0.45f) / 0.30f;
        r = (uint8_t)(255 * f);
        g = (uint8_t)(255 - (85 * f));
        b = 0;
      }
      else
      {
        // orange -> red
        float f = (ratio - 0.75f) / 0.25f;
        r = 255;
        g = (uint8_t)(170 * (1.0f - f));
        b = 0;
      }

      dsc_bar.bg_color = lv_color_make(r, g, b);

      int y_pos = CANVAS_HEIGHT - y - 1;
      lv_area_t pixel_bar = {
          x, y_pos,
          x + BAR_WIDTH - 2, y_pos};
      lv_draw_rect(&layer, &dsc_bar, &pixel_bar);
    }

    uint8_t new_peak_y = y_start;
    if (peak_y[i] == 0 || new_peak_y < peak_y[i])
    {
      // Start peak slightly above the bar top so the fall is more visible.
      peak_y[i] = (new_peak_y > 2) ? (new_peak_y - 2) : 0;
      peak_hold[i] = 10;
    }
    else
    {
      if (peak_hold[i] > 0)
      {
        peak_hold[i]--;
      }
      else
      {
        peak_y[i] += 1;
      }
      if (peak_y[i] > CANVAS_HEIGHT - 2)
        peak_y[i] = CANVAS_HEIGHT - 2;
    }

    lv_area_t peak_area = {x, peak_y[i], x + BAR_WIDTH - 2, peak_y[i] + 1};
    lv_draw_rect(&layer, &dsc_peak, &peak_area);
  }

  // Adaptive scale (FFT mode only): fast rise, gentle decay
  if (!use_vu_fallback && frame_max > fft_auto_max)
  {
    fft_auto_max = frame_max;
  }
  else if (!use_vu_fallback)
  {
    fft_auto_max = (fft_auto_max * 7 + frame_max) / 8;
  }
  if (!use_vu_fallback && fft_auto_max < 40)
    fft_auto_max = 40;

  lv_canvas_finish_layer(canvas, &layer);
}

void debug_canvas_heartbeat()
{
#if DEBUG_CANVAS_HEARTBEAT
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

static inline void fft_push_mono_sample(float mono)
{
  static float previous_sample = 0;
  static constexpr float alpha = 0.2f;
  static constexpr float gain = 1.0f;

  float filtered = previous_sample + alpha * (mono - previous_sample);
  previous_sample = filtered;

  int32_t processed = (int32_t)(filtered * gain);
  if (processed > 32767)
    processed = 32767;
  if (processed < -32768)
    processed = -32768;

  if (fft_index < FFT_SIZE * 2)
  {
    fft_buffer[fft_index++] = (int16_t)processed;
  }
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
    ready_to_fft.store(true, std::memory_order_release);
    fft_exec_calls.fetch_add(1, std::memory_order_relaxed);
  }
}

static void handle_audio_process_raw_samples(int32_t *outBuff, int32_t validSamples)
{
  fft_raw_active.store(true, std::memory_order_relaxed);
  fft_raw_calls.fetch_add(1, std::memory_order_relaxed);
  fft_cb_calls.fetch_add(1, std::memory_order_relaxed);

  fft_process_from_int32_stereo(outBuff, validSamples);
}

static void handle_audio_process_i2s(int32_t *outBuff, int32_t validSamples, bool *continueI2S)
{
  fft_i2s_calls.fetch_add(1, std::memory_order_relaxed);
  if (fft_raw_active.load(std::memory_order_relaxed))
  {
    *continueI2S = true;
    return;
  }

  fft_cb_calls.fetch_add(1, std::memory_order_relaxed);
  fft_process_from_int32_stereo(outBuff, validSamples);

  *continueI2S = true;
}

// Force exact symbol names of ESP32-audioI2S weak callbacks (C++ mangled):
// _Z25audio_process_raw_samplesPls  -> void audio_process_raw_samples(long*, short)
// _Z17audio_process_i2sPlsPb       -> void audio_process_i2s(long*, short, bool*)
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

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

static void setLabelTextAutoScroll(lv_obj_t *label, const char *text)
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

  // Important: scrolling needs a constrained label width (not LV_SIZE_CONTENT).
  lv_obj_set_width(label, max_w);
  // Keep text centered in the bounded label area.
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

  const lv_font_t *font = (const lv_font_t *)lv_obj_get_style_text_font(label, LV_PART_MAIN);
  lv_point_t size;
  lv_text_get_size(&size, text, font, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

  Serial.printf("[LBL] text_px=%d content_w=%d obj_w=%d text_len=%d\n",
                (int)size.x,
                (int)lv_obj_get_content_width(label),
                (int)lv_obj_get_width(label),
                (int)strlen(text));

  if (size.x > max_w)
  {
    // Scroll left-right for long titles.
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_anim_duration(label, 5000, LV_PART_MAIN);
    Serial.println("[LBL] mode=SCROLL");
  }
  else
  {
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
    Serial.println("[LBL] mode=CLIP");
  }

  lv_label_set_text(label, text);
}

static void processPendingLabelUpdates()
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
    setLabelTextAutoScroll(ui_LblStation, stationLocal);
  if (doTitle)
    setLabelTextAutoScroll(ui_LblCurPlaying, titleLocal);
}

// Print station info
void my_audio_info(Audio::msg_t m)
{
  // Serial.printf("%s: %s\n", m.s, m.msg);
  switch (m.e)
  {
  case Audio::evt_name:
    Serial.printf("station name: %s\n", m.msg);
    {
      std::lock_guard<std::mutex> lock(uiTextMutex);
      strncpy(pendingStationText, m.msg ? m.msg : "", sizeof(pendingStationText) - 1);
      pendingStationText[sizeof(pendingStationText) - 1] = '\0';
      pendingStationUpdate.store(true, std::memory_order_release);
    }
    break;
  case Audio::evt_streamtitle:
    Serial.printf("stream title: %s\n", m.msg);
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

  // SSID je prázdný? → nic uloženého není
  if (strlen((char *)conf.sta.ssid) == 0)
  {
    return false;
  }
  return true;
}

void connecting_animation(lv_timer_t *timer)
{
  static char buff[32];

  dotCount = (dotCount + 1) % 4; // 0,1,2,3 → pak zpět
  snprintf(buff, sizeof(buff), "Connecting%s",
           dotCount == 0 ? "" : dotCount == 1 ? "."
                            : dotCount == 2   ? ".."
                                              : "...");

  lv_label_set_text(ui_LblInfo, buff);
}

static void setupOtaUpdate()
{
  if (otaInitialized)
    return;

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("OTA not started: WiFi not connected");
    return;
  }

  const esp_partition_t *nextUpdatePart = esp_ota_get_next_update_partition(NULL);
  if (!nextUpdatePart)
  {
    Serial.println("OTA not available: no OTA update partition found");
    return;
  }

  ArduinoOTA.setHostname(OTA_HOSTNAME);
  if (strlen(OTA_PASSWORD) > 0)
    ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA start");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA end");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    if (total > 0)
      Serial.printf("OTA progress: %u%%\r", (progress * 100U) / total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA error[%u]\n", (unsigned int)error);
  });

  ArduinoOTA.begin();
  otaInitialized = true;
  Serial.printf("OTA ready: %s.local\n", OTA_HOSTNAME);
}

void connectToWiFi()
{
  // 2) Spustíme boot screen + animaci
  //lv_scr_load(ui_ScrBoot);
  lv_label_set_text(ui_LblInfo, "Connecting");

  connecting_timer = lv_timer_create(connecting_animation, 400, NULL);
  
  // // 1) Zkontrolujeme, jestli v NVS existují uložené údaje
  // if (!wifiCredentialsStored())
  // {
  //   Serial.println("No WiFi credentials → launching WiFiManager");

  //   lv_scr_load(ui_ScrWiFiManager);

  //   wm.autoConnect("MusicPlayerAP", "password");

  //   return; // po konfiguraci WiFiManager sám uloží data do NVS
  // }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true, true);
  WiFi.begin();

  uint32_t start = millis();
  const uint32_t timeout = 8000;

  // 3) Pokus o spojení
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeout)
  {
    lv_timer_handler();
    delay(5);
  }

  // 4) Výsledek
  lv_timer_del(connecting_timer);

  if (WiFi.status() == WL_CONNECTED)
  {
    lv_label_set_text(ui_LblInfo, "Connected!");
    delay(600);

    lv_scr_load(ui_ScrRadioPlayer);
  }
  else
  {
    lv_label_set_text(ui_LblInfo, "Failed → WiFi Manager");
    lv_scr_load(ui_ScrWiFiManager);

    delay(800);

    wm.autoConnect("MusicPlayerAP", "password");
  }
}

void syncTime()
{
  // set notification call-back function
  // sntp_set_time_sync_notification_cb(timeavailable);

  /**
   * NTP server address could be aquired via DHCP,
   *
   * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
   * otherwise SNTP option 42 would be rejected by default.
   * NOTE: configTime() function call if made AFTER DHCP-client run
   * will OVERRIDE aquired NTP server address
   */
  // sntp_servermode_dhcp(1);  // (optional)

  /**
   * This will set configured ntp servers and constant TimeZone/daylightOffset
   * should be OK if your time zone does not need to adjust daylightOffset twice a year,
   * in such a case time adjustment won't be handled automagicaly.
   */
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  /**
   * A more convenient approach to handle TimeZones with daylightOffset
   * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
   * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
   */
  // configTzTime(time_zone, ntpServer1, ntpServer2);

  /*
      Sync time with NTP server and update ESP32 RTC
      getLocalTime() return false if time is not set
*/

  Serial.println("Syncing time with NTP server..");
  lv_label_set_text(ui_LblInfo, "Syncing time..");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo))
  {
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

  // Start 1-second software tick from current runtime point.
  targetCountTime = millis() + 1000;

  Serial.println("");
  printLocalTime();
}

void displaySetup()
{

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
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
#else  // !(defined(DIRECT_MODE) && (defined(CANVAS) || defined(RGB_PANEL)))
  // Prefer PSRAM for LVGL draw buffer to keep internal heap free for TLS.
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
  {
    Serial.println("LVGL draw buffer PSRAM alloc failed, trying internal RAM...");
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  }
  if (!disp_draw_buf)
  {
    // Last fallback: any 8-bit capable RAM.
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
#endif // !(defined(DIRECT_MODE) && (defined(CANVAS) || defined(RGB_PANEL)))
#else  // !ESP32
  Serial.println("LVGL disp_draw_buf heap_caps_malloc failed! malloc again...");
  disp_draw_buf = (lv_color_t *)malloc(bufSize * 2);
#endif // !ESP32
  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
#ifdef DIRECT_MODE
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
  }
}

void setup()
{
  Audio::audio_info_callback = my_audio_info;
  Serial.begin(115200);

  initButtons();
#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  String LVGL_version = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_version);

  Serial.printf("PSRAM: %d bytes\n", ESP.getPsramSize());
Serial.printf("Free PSRAM: %d bytes\n", ESP.getFreePsram());


  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  // Keep volume scale consistent across audio library versions.
  audio.setVolumeSteps(21);

  // Initial volume
  audio.setVolume(7);

  encoder.begin();

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  displaySetup();

  // Initialize the Rotary Encoder input device.
  indev_encoder = lv_indev_create();
  lv_indev_set_type(indev_encoder, LV_INDEV_TYPE_ENCODER);
  lv_indev_set_read_cb(indev_encoder, encoder_read);

  ui_init();
  ensureModeScreensUi();

  connectToWiFi();
  setupOtaUpdate();
  Serial.print("Connecting to saved WiFi credentials\n");

  // WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }

  syncTime();

  //lv_scr_load(ui_ScrRadioPlayer);

  // mapping of hw buttons to LVGL buttons
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

  LV_DRAW_BUF_DEFINE_STATIC(canvas_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_RGB565);
  LV_DRAW_BUF_INIT_STATIC(canvas_buf);

  /*Create a canvas and initialize its palette*/
  canvas = lv_canvas_create(lv_screen_active());
  lv_canvas_set_draw_buf(canvas, &canvas_buf);
  lv_obj_set_parent(canvas, ui_CntnrRadio);
  lv_obj_align(canvas, LV_ALIGN_BOTTOM_MID, 0, -5);
  lv_obj_move_foreground(canvas);
  lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_TRANSP);

  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);
  lv_canvas_finish_layer(canvas, &layer);

  char numberMin[3], numberHrs[3];
  char numberDate[11];
  sprintf(numberDate, "%02d.%02d.%04d", day, month, year);
  lv_label_set_text(ui_LblDate, numberDate);
  sprintf(numberMin, "%02d", minutes);
  lv_label_set_text(ui_LblMin, numberMin);
  sprintf(numberHrs, "%02d", hour);
  lv_label_set_text(ui_LblHrs, numberHrs);

  uint8_t station = loadStation();

  lv_obj_t *restoredButton = getStationButton(station);
  if (restoredButton != nullptr)
  {
    lv_obj_send_event(restoredButton, LV_EVENT_CLICKED, NULL);
  }
  else
  {
    connectToStation(station);
  }
  currentStation = station;

  setPlayerMode(MODE_RADIO);
  updateWiFiSignalIcon();

  Serial.println("Setup done");
}

void loop()
{

  audio.loop();
  wm.process();

  if (!otaInitialized)
  {
    setupOtaUpdate();
  }
  if (otaInitialized)
  {
    ArduinoOTA.handle();
  }

  processPendingLabelUpdates();

  unsigned long now = millis();
  if ((now - lastWifiSignalCheck) >= 5000)
  {
    lastWifiSignalCheck = now;
    updateWiFiSignalIcon();
  }

  debug_canvas_heartbeat();

  static unsigned long last_update = 0;
  const unsigned long update_interval = 33; // ms (~30 FPS)
  if (playerMode == MODE_RADIO && millis() - last_update >= update_interval)
  {
    last_update = millis();
    if (ready_to_fft.exchange(false, std::memory_order_acq_rel))
    {
    }
    draw_fft_level_meter_lvgl(canvas);
  }

  if (playerMode == MODE_SD_MP3)
  {
    updateSdPlayTimeSlider();

    bool running = audio.isRunning();
    const unsigned long sdAutoplayGraceMs = 1500;
    if (sdTrackLoaded && !sdPlaybackPaused && sdWasRunning && !running &&
        (millis() - sdTrackStartMs) > sdAutoplayGraceMs)
    {
      int nextTrack = findNextSdTrackIndex(currentSdTrack);
      if (nextTrack >= 0)
      {
        playSdTrack(nextTrack);
        running = audio.isRunning();
      }
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

#if FFT_DEBUG_LOG
  static unsigned long last_fft_debug = 0;
  if (millis() - last_fft_debug >= 1000)
  {
    last_fft_debug = millis();
    uint16_t vu = audio.getVUlevel();
    Serial.printf("FFT dbg: cb=%lu exec=%lu raw=%lu i2s=%lu vuL=%u vuR=%u autoMax=%lu bin1=%lu\n",
                  (unsigned long)fft_cb_calls.load(std::memory_order_relaxed),
                  (unsigned long)fft_exec_calls.load(std::memory_order_relaxed),
                  (unsigned long)fft_raw_calls.load(std::memory_order_relaxed),
                  (unsigned long)fft_i2s_calls.load(std::memory_order_relaxed),
                  (unsigned int)(vu & 0xFF),
                  (unsigned int)((vu >> 8) & 0xFF),
                  (unsigned long)fft_auto_max,
                  (unsigned long)fft.get(1));
  }
#endif

  readVolumeValue();
  lv_task_handler(); /* let the GUI do its work */
  processButtons();
  countTime();
  processEncoder();

#ifdef DIRECT_MODE
#if defined(CANVAS) || defined(RGB_PANEL)
  gfx->flush();
#else  // !(defined(CANVAS) || defined(RGB_PANEL))
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)disp_draw_buf, screenWidth, screenHeight);
#endif // !(defined(CANVAS) || defined(RGB_PANEL))
#else  // !DIRECT_MODE
#ifdef CANVAS
  gfx->flush();
#endif
#endif // !DIRECT_MODE
}
