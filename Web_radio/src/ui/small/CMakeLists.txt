SET(SOURCES ui_ScrBoot.c
    ui_ScrRadioPlayer.c
    ui_ScrMusicPlayer.c
    ui_ScrNetworkPlayer.c
    ui_ScrWiFiManager.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_music_player_logo_png.c
    ui_img_weather_icons_dovora_interactive_128px_day_clear_png.c
    ui_img_weather_icons_dovora_interactive_128px_cloudy_png.c
    ui_img_indicator_hor_small_png.c
    fonts/ui_font_Roboto_Reg_10.c
    fonts/ui_font_Roboto_Reg_12.c
    fonts/ui_font_Roboto_Reg_14.c
    fonts/ui_font_Roboto_Reg_16.c
    fonts/ui_font_Roboto_Reg_18.c
    fonts/ui_font_Roboto_Reg_28.c
    fonts/ui_font_Roboto_Reg_48.c)

add_library(ui ${SOURCES})
