SET(SOURCES ui_Screen1.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_indicator_hor_small_png.c
    fonts/ui_font_Roboto_Reg_18.c
    fonts/ui_font_Roboto_Reg_28.c
    fonts/ui_font_Roboto_Reg_48.c)

add_library(ui ${SOURCES})
