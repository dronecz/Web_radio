#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <lvgl.h>

extern lv_obj_t *canvas;
extern lv_timer_t *connecting_timer;
extern uint8_t dotCount;
extern int modeMenuSelection;
extern unsigned long lastWifiSignalCheck;

void my_print(lv_log_level_t level, const char *buf);
uint32_t millis_cb(void);
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
void encoder_read(lv_indev_t *indev, lv_indev_data_t *data);
void displaySetup();
void draw_fft_level_meter_lvgl(lv_obj_t *canvas);
void debug_canvas_heartbeat();
void processPendingLabelUpdates();
void my_audio_info(Audio::msg_t m);
void connecting_animation(lv_timer_t *timer);
void ensureModeScreensUi();
void updateWiFiSignalIcon();
void ensureVolumeSliderVisibleOnActiveScreen();
void setModeInfoLabel(lv_obj_t *label, const char *text);
bool isModeMenuVisible();
void hideModeMenu();
void showModeMenu();
lv_obj_t *getModeMenuButtonBySelection(int sel);
void updateModeMenuSelectionVisual();
void createSkinMenuUI();
void updateSkinMenuLabel();
void cycleSkinUIFromButton(lv_event_t *e);
