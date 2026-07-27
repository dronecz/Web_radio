#include "mp3_player.h"
#include "app_shared.h"
#include "gui.h"
#include "ui_select.h"
#include <Audio.h>
#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include <algorithm>
#include <functional>
#include <vector>

extern Audio audio;
extern PlayerMode playerMode;
extern lv_obj_t *ui_Button9;
extern lv_obj_t *ui_Button10;
extern lv_obj_t *ui_Button11;
extern lv_obj_t *ui_Button12;
extern lv_obj_t *ui_Button14;
extern lv_obj_t *ui_Image1;
extern lv_obj_t *ui_Image2;
extern lv_obj_t *ui_LblCurPlayedTime;
extern lv_obj_t *ui_LblSongDuration;
extern lv_obj_t *ui_SliderPlayTime;
extern lv_obj_t *ui_ModeMusicInfo;
extern lv_obj_t *ui_Container6;

const int buttonCount = 5;
const int buttonPins[buttonCount] = {40, 41, 42, 44, 43};

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

static const uint8_t SD_CS_PIN = 2;
static const uint8_t SD_MOSI_PIN = 11;
static const uint8_t SD_MISO_PIN = 13;
static const uint8_t SD_SCK_PIN = 12;

static String getFolderFromPath(const String &path)
{
  int slash = path.lastIndexOf('/');
  if (slash <= 0)
    return String("/");
  return path.substring(0, slash);
}

void updateSdFolderButtonsState()
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

void updateSdPlayPauseVisual()
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

void updateSdPlayTimeSlider()
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

  return currentIndex + 1;
}

static inline bool hasAudioExtension(const String &name)
{
  String lower = name;
  lower.toLowerCase();
  return lower.endsWith(".mp3") || lower.endsWith(".aac") || lower.endsWith(".wav") || lower.endsWith(".m4a") || lower.endsWith(".flac");
}

bool initAndScanSD()
{
  if (!sdInitialized)
  {
    if (!sdSpiInitialized)
    {
      SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
      sdSpiInitialized = true;
    }

    sdInitialized = SD.begin(SD_CS_PIN, SPI, 16000000);
    if (!sdInitialized)
    {
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
      else if (hasAudioExtension(entryPath))
      {
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

void playSdTrack(int index)
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

bool jumpSdFolder(int direction)
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
