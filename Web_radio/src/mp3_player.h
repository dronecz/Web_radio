#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include <vector>

extern std::vector<String> sdTracks;
extern std::vector<String> sdFolders;
extern int currentSdTrack;
extern int currentSdFolder;
extern bool sdInitialized;
extern bool sdScanned;
extern bool sdSpiInitialized;
extern bool sdTrackLoaded;
extern bool sdPlaybackPaused;
extern bool sdWasRunning;
extern unsigned long sdTrackStartMs;

void updateSdFolderButtonsState();
void updateSdPlayPauseVisual();
void updateSdPlayTimeSlider();
bool initAndScanSD();
void playSdTrack(int index);
bool jumpSdFolder(int direction);
void music_player_button_event_handler(lv_event_t *e);
