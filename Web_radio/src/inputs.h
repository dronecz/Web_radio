#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <lvgl.h>

void ensureVolumeSliderVisibleOnActiveScreen();
bool isModeMenuVisible();
void showModeMenu();
void updateModeMenuSelectionVisual();
lv_obj_t *getModeMenuButtonBySelection(int sel);
extern int modeMenuSelection;

void initButtons();
void processButtons();
void processEncoder();
int read_potentiometer();
void readVolumeValue();
void btn_event_handler(lv_event_t *e);
void mode_button_event_handler(lv_event_t *e);
