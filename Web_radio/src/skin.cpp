#include "skin.h"
#include <Preferences.h>

// ============ Built-in Skin Definitions ============

// Dark theme (current default)
const Skin skin_dark = {
    .name = "Dark",
    .colors = {
        .bg_primary = 0x000000,
        .bg_secondary = 0x1A1A1A,
        .button_bg = 0xDADADA,
        .button_border = 0x585858,
        .button_checked = 0x969696,
        .text_primary = 0xFFFFFF,
        .text_secondary = 0xB0B0B0,
        .accent = 0x2CC601,
        .fft_bar_low = 0x00FFFF,
        .fft_bar_high = 0xFF0000,
        .wifi_icon_error = 0xFF0000,
        .divider = 0x313131,
    },
    .dims = {
        .button_height = 30,
        .button_border_width = 2,
        .button_radius = 5,
        .divider_width = 2,
    }};

// Light theme
const Skin skin_light = {
    .name = "Light",
    .colors = {
        .bg_primary = 0xFFFFFF,
        .bg_secondary = 0xF0F0F0,
        .button_bg = 0xE8E8E8,
        .button_border = 0x808080,
        .button_checked = 0xC0C0C0,
        .text_primary = 0x000000,
        .text_secondary = 0x505050,
        .accent = 0x0080FF,
        .fft_bar_low = 0x0080FF,
        .fft_bar_high = 0xFF0000,
        .wifi_icon_error = 0xFF0000,
        .divider = 0xD0D0D0,
    },
    .dims = {
        .button_height = 30,
        .button_border_width = 2,
        .button_radius = 5,
        .divider_width = 2,
    }};

// Neon theme (high contrast)
const Skin skin_neon = {
    .name = "Neon",
    .colors = {
        .bg_primary = 0x0A0E27,
        .bg_secondary = 0x16213E,
        .button_bg = 0x0F3460,
        .button_border = 0x00D9FF,
        .button_checked = 0x00FF00,
        .text_primary = 0x00FF00,
        .text_secondary = 0x00D9FF,
        .accent = 0xFF006E,
        .fft_bar_low = 0x00FF00,
        .fft_bar_high = 0xFF006E,
        .wifi_icon_error = 0xFF006E,
        .divider = 0x00D9FF,
    },
    .dims = {
        .button_height = 30,
        .button_border_width = 2,
        .button_radius = 3,
        .divider_width = 2,
    }};

// ============ SkinManager Implementation ============

SkinManager::SkinManager() : skin_count(0), current_skin_index(0)
{
  // Register built-in skins
  registerSkin(&skin_dark);
  registerSkin(&skin_light);
  registerSkin(&skin_neon);
}

SkinManager &SkinManager::instance()
{
  static SkinManager mgr;
  return mgr;
}

void SkinManager::registerSkin(const Skin *skin)
{
  if (skin_count < MAX_SKINS && skin != nullptr)
  {
    skins[skin_count] = skin;
    skin_count++;
  }
}

const Skin *SkinManager::getSkin(int index) const
{
  if (index >= 0 && index < skin_count)
    return skins[index];
  return nullptr;
}

const Skin *SkinManager::getCurrentSkin() const
{
  return getSkin(current_skin_index);
}

int SkinManager::getSkinCount() const
{
  return skin_count;
}

int SkinManager::getCurrentSkinIndex() const
{
  return current_skin_index;
}

void SkinManager::setSkin(int index)
{
  if (index >= 0 && index < skin_count)
  {
    current_skin_index = index;
    const Skin *skin = getSkin(index);
    if (skin)
    {
      applySkinToUI(skin);
      saveSkinPreference();
    }
  }
}

void SkinManager::saveSkinPreference()
{
  Preferences prefs;
  prefs.begin("ui", false);
  prefs.putUChar("skin", (uint8_t)current_skin_index);
  prefs.end();
  Serial.printf("[SKIN] Saved skin preference: %d\n", current_skin_index);
}

void SkinManager::loadSkinPreference()
{
  Preferences prefs;
  prefs.begin("ui", true);
  uint8_t saved_skin = prefs.getUChar("skin", 0);
  prefs.end();

  if (saved_skin < (uint8_t)skin_count)
  {
    current_skin_index = saved_skin;
    Serial.printf("[SKIN] Loaded skin preference: %d\n", current_skin_index);
  }
  else
  {
    current_skin_index = 0;
  }
}

// ============ UI Application ============

extern lv_obj_t *ui_Button1;
extern lv_obj_t *ui_Button2;
extern lv_obj_t *ui_Button3;
extern lv_obj_t *ui_Button4;
extern lv_obj_t *ui_Button5;
extern lv_obj_t *ui_Button6;
extern lv_obj_t *ui_Button7;
extern lv_obj_t *ui_Button8;
extern lv_obj_t *ui_Button9;
extern lv_obj_t *ui_Button10;
extern lv_obj_t *ui_Button11;
extern lv_obj_t *ui_Button12;
extern lv_obj_t *ui_Button14;
extern lv_obj_t *ui_ScrRadioPlayer;
extern lv_obj_t *ui_ScrMusicPlayer;
extern lv_obj_t *ui_ScrNetworkPlayer;
extern lv_obj_t *ui_ScrWiFiManager;
extern lv_obj_t *ui_PnlHorLine;
extern lv_obj_t *ui_PnlHorLinePlayer;
extern lv_obj_t *ui_PnlVerLine;

static void apply_button_style(lv_obj_t *btn, const SkinColorPalette &colors, const SkinDimensions &dims)
{
  if (!btn)
    return;

  lv_obj_set_height(btn, dims.button_height);
  lv_obj_set_style_radius(btn, dims.button_radius, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(btn, lv_color_hex(colors.button_bg), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(btn, lv_color_hex(colors.button_border), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(btn, dims.button_border_width, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(btn, lv_color_hex(colors.button_checked), LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_CHECKED);
}

static void apply_divider_style(lv_obj_t *divider, const SkinColorPalette &colors, const SkinDimensions &dims)
{
  if (!divider)
    return;

  lv_obj_set_style_bg_color(divider, lv_color_hex(colors.divider), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(divider, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void apply_screen_style(lv_obj_t *screen, const SkinColorPalette &colors)
{
  if (!screen)
    return;

  lv_obj_set_style_bg_color(screen, lv_color_hex(colors.bg_primary), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void applySkinToUI(const Skin *skin)
{
  if (!skin)
    return;

  const SkinColorPalette &colors = skin->colors;
  const SkinDimensions &dims = skin->dims;

  Serial.printf("[SKIN] Applying skin: %s\n", skin->name);

  // Apply to screens
  apply_screen_style(ui_ScrRadioPlayer, colors);
  apply_screen_style(ui_ScrMusicPlayer, colors);
  apply_screen_style(ui_ScrNetworkPlayer, colors);
  apply_screen_style(ui_ScrWiFiManager, colors);

  // Apply to buttons
  apply_button_style(ui_Button1, colors, dims);
  apply_button_style(ui_Button2, colors, dims);
  apply_button_style(ui_Button3, colors, dims);
  apply_button_style(ui_Button4, colors, dims);
  apply_button_style(ui_Button5, colors, dims);
  apply_button_style(ui_Button6, colors, dims);
  apply_button_style(ui_Button7, colors, dims);
  apply_button_style(ui_Button8, colors, dims);
  apply_button_style(ui_Button9, colors, dims);
  apply_button_style(ui_Button10, colors, dims);
  apply_button_style(ui_Button11, colors, dims);
  apply_button_style(ui_Button12, colors, dims);
  apply_button_style(ui_Button14, colors, dims);

  // Apply to dividers
  apply_divider_style(ui_PnlHorLine, colors, dims);
  apply_divider_style(ui_PnlHorLinePlayer, colors, dims);
  apply_divider_style(ui_PnlVerLine, colors, dims);
}

void cycleSkin()
{
  SkinManager &mgr = SkinManager::instance();
  int next = (mgr.getCurrentSkinIndex() + 1) % mgr.getSkinCount();
  mgr.setSkin(next);
}

const char *getCurrentSkinName()
{
  const Skin *skin = SkinManager::instance().getCurrentSkin();
  return skin ? skin->name : "Unknown";
}
