#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

struct SkinColorPalette
{
  uint32_t bg_primary;           // Main background
  uint32_t bg_secondary;         // Secondary/container background
  uint32_t button_bg;            // Button background
  uint32_t button_border;        // Button border
  uint32_t button_checked;       // Button when checked
  uint32_t text_primary;         // Primary text
  uint32_t text_secondary;       // Secondary text
  uint32_t accent;               // Accent color
  uint32_t fft_bar_low;          // FFT bar low frequency
  uint32_t fft_bar_high;         // FFT bar high frequency
  uint32_t wifi_icon_error;      // WiFi icon error state
  uint32_t divider;              // Divider line color
};

struct SkinDimensions
{
  lv_coord_t button_height;
  lv_coord_t button_border_width;
  lv_coord_t button_radius;
  lv_coord_t divider_width;
};

struct Skin
{
  const char *name;
  SkinColorPalette colors;
  SkinDimensions dims;
};

// Skin manager
class SkinManager
{
public:
  static SkinManager &instance();

  void registerSkin(const Skin *skin);
  const Skin *getSkin(int index) const;
  const Skin *getCurrentSkin() const;
  int getSkinCount() const;
  int getCurrentSkinIndex() const;

  void setSkin(int index);
  void saveSkinPreference();
  void loadSkinPreference();

private:
  SkinManager();
  static const int MAX_SKINS = 8;
  const Skin *skins[MAX_SKINS];
  int skin_count;
  int current_skin_index;
};

// Built-in skins
extern const Skin skin_dark;
extern const Skin skin_light;
extern const Skin skin_neon;

// Apply skin to UI elements
void applySkinToUI(const Skin *skin);

// Cycle to next skin and apply it
void cycleSkin();

// Get current skin name
const char *getCurrentSkinName();
