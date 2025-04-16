// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 9.1.0
// Project name: Web_radio

#include "ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_CntnrTime = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_CntnrTime);
    lv_obj_set_width(ui_CntnrTime, 235);
    lv_obj_set_height(ui_CntnrTime, 155);
    lv_obj_remove_flag(ui_CntnrTime, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LblHrs = lv_label_create(ui_CntnrTime);
    lv_obj_set_width(ui_LblHrs, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblHrs, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblHrs, -40);
    lv_obj_set_y(ui_LblHrs, 0);
    lv_obj_set_align(ui_LblHrs, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblHrs, "23");
    lv_obj_set_style_text_font(ui_LblHrs, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblMin = lv_label_create(ui_CntnrTime);
    lv_obj_set_width(ui_LblMin, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblMin, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblMin, 40);
    lv_obj_set_y(ui_LblMin, 0);
    lv_obj_set_align(ui_LblMin, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblMin, "59");
    lv_obj_set_style_text_font(ui_LblMin, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblDots = lv_label_create(ui_CntnrTime);
    lv_obj_set_width(ui_LblDots, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblDots, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblDots, 0);
    lv_obj_set_y(ui_LblDots, -5);
    lv_obj_set_align(ui_LblDots, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblDots, ":");
    lv_obj_set_style_text_font(ui_LblDots, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblDate = lv_label_create(ui_CntnrTime);
    lv_obj_set_width(ui_LblDate, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblDate, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblDate, 0);
    lv_obj_set_y(ui_LblDate, 40);
    lv_obj_set_align(ui_LblDate, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblDate, "31.12.2025");
    lv_obj_set_style_text_font(ui_LblDate, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_CntnrWeather = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_CntnrWeather);
    lv_obj_set_width(ui_CntnrWeather, 235);
    lv_obj_set_height(ui_CntnrWeather, 155);
    lv_obj_set_align(ui_CntnrWeather, LV_ALIGN_TOP_RIGHT);
    lv_obj_remove_flag(ui_CntnrWeather, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_CntnrRadio = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_CntnrRadio);
    lv_obj_set_height(ui_CntnrRadio, 110);
    lv_obj_set_width(ui_CntnrRadio, lv_pct(100));
    lv_obj_set_x(ui_CntnrRadio, 0);
    lv_obj_set_y(ui_CntnrRadio, -50);
    lv_obj_set_align(ui_CntnrRadio, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_flag(ui_CntnrRadio, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_CntnrVisualisation = lv_obj_create(ui_CntnrRadio);
    lv_obj_remove_style_all(ui_CntnrVisualisation);
    lv_obj_set_width(ui_CntnrVisualisation, lv_pct(100));
    lv_obj_set_height(ui_CntnrVisualisation, lv_pct(70));
    lv_obj_set_align(ui_CntnrVisualisation, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_flag(ui_CntnrVisualisation, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LblStation = lv_label_create(ui_CntnrRadio);
    lv_obj_set_width(ui_LblStation, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblStation, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblStation, 0);
    lv_obj_set_y(ui_LblStation, 15);
    lv_obj_set_align(ui_LblStation, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_LblStation, "Radio Station");
    lv_obj_set_style_text_color(ui_LblStation, lv_color_hex(0xBEBEBE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblStation, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LblStation, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblCurPlaying = lv_label_create(ui_CntnrRadio);
    lv_obj_set_width(ui_LblCurPlaying, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblCurPlaying, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblCurPlaying, 0);
    lv_obj_set_y(ui_LblCurPlaying, 10);
    lv_obj_set_align(ui_LblCurPlaying, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_LblCurPlaying, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_LblCurPlaying, "Currently Playing");
    lv_obj_set_style_text_font(ui_LblCurPlaying, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_CntnrButtons = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_CntnrButtons);
    lv_obj_set_height(ui_CntnrButtons, 40);
    lv_obj_set_width(ui_CntnrButtons, lv_pct(100));
    lv_obj_set_align(ui_CntnrButtons, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_flag(ui_CntnrButtons, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Button1 = lv_button_create(ui_CntnrButtons);
    lv_obj_set_width(ui_Button1, 60);
    lv_obj_set_height(ui_Button1, 30);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0xDADADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button1, lv_color_hex(0x585858), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0x969696), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button1, 0, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Container1 = lv_obj_create(ui_Button1);
    lv_obj_remove_style_all(ui_Container1);
    lv_obj_set_height(ui_Container1, 5);
    lv_obj_set_width(ui_Container1, lv_pct(125));
    lv_obj_set_x(ui_Container1, 0);
    lv_obj_set_y(ui_Container1, -5);
    lv_obj_set_align(ui_Container1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Container1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Container1, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Container1, lv_color_hex(0x166600), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Container1, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Button2 = lv_button_create(ui_CntnrButtons);
    lv_obj_set_width(ui_Button2, 60);
    lv_obj_set_height(ui_Button2, 30);
    lv_obj_set_x(ui_Button2, -80);
    lv_obj_set_y(ui_Button2, 0);
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0xDADADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button2, lv_color_hex(0x585858), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0x969696), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button2, 0, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Container2 = lv_obj_create(ui_Button2);
    lv_obj_remove_style_all(ui_Container2);
    lv_obj_set_height(ui_Container2, 5);
    lv_obj_set_width(ui_Container2, lv_pct(125));
    lv_obj_set_x(ui_Container2, 0);
    lv_obj_set_y(ui_Container2, -5);
    lv_obj_set_align(ui_Container2, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Container2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Container2, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Container2, lv_color_hex(0x166600), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Container2, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Button3 = lv_button_create(ui_CntnrButtons);
    lv_obj_set_width(ui_Button3, 60);
    lv_obj_set_height(ui_Button3, 30);
    lv_obj_set_x(ui_Button3, -160);
    lv_obj_set_y(ui_Button3, 0);
    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button3, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button3, lv_color_hex(0xDADADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button3, lv_color_hex(0x585858), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button3, lv_color_hex(0x969696), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button3, 0, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Container3 = lv_obj_create(ui_Button3);
    lv_obj_remove_style_all(ui_Container3);
    lv_obj_set_height(ui_Container3, 5);
    lv_obj_set_width(ui_Container3, lv_pct(125));
    lv_obj_set_x(ui_Container3, 0);
    lv_obj_set_y(ui_Container3, -5);
    lv_obj_set_align(ui_Container3, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Container3, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Container3, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Container3, lv_color_hex(0x166600), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Container3, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Button4 = lv_button_create(ui_CntnrButtons);
    lv_obj_set_width(ui_Button4, 60);
    lv_obj_set_height(ui_Button4, 30);
    lv_obj_set_x(ui_Button4, 80);
    lv_obj_set_y(ui_Button4, 0);
    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button4, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Button4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button4, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(0xDADADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button4, lv_color_hex(0x585858), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(0x969696), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button4, 0, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Container4 = lv_obj_create(ui_Button4);
    lv_obj_remove_style_all(ui_Container4);
    lv_obj_set_height(ui_Container4, 5);
    lv_obj_set_width(ui_Container4, lv_pct(125));
    lv_obj_set_x(ui_Container4, 0);
    lv_obj_set_y(ui_Container4, -5);
    lv_obj_set_align(ui_Container4, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Container4, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Container4, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Container4, lv_color_hex(0x166600), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Container4, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Button5 = lv_button_create(ui_CntnrButtons);
    lv_obj_set_width(ui_Button5, 60);
    lv_obj_set_height(ui_Button5, 30);
    lv_obj_set_x(ui_Button5, 160);
    lv_obj_set_y(ui_Button5, 0);
    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button5, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Button5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button5, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0xDADADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button5, lv_color_hex(0x585858), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0x969696), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button5, 0, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Container5 = lv_obj_create(ui_Button5);
    lv_obj_remove_style_all(ui_Container5);
    lv_obj_set_height(ui_Container5, 5);
    lv_obj_set_width(ui_Container5, lv_pct(125));
    lv_obj_set_x(ui_Container5, 0);
    lv_obj_set_y(ui_Container5, -5);
    lv_obj_set_align(ui_Container5, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Container5, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Container5, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Container5, lv_color_hex(0x166600), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Container5, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_PnlHorLine = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_PnlHorLine, 2);
    lv_obj_set_width(ui_PnlHorLine, lv_pct(90));
    lv_obj_set_align(ui_PnlHorLine, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PnlHorLine, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PnlHorLine, lv_color_hex(0x313131), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PnlHorLine, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PnlVerLine = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_PnlVerLine, 2);
    lv_obj_set_height(ui_PnlVerLine, 130);
    lv_obj_set_x(ui_PnlVerLine, 0);
    lv_obj_set_y(ui_PnlVerLine, 20);
    lv_obj_set_align(ui_PnlVerLine, LV_ALIGN_TOP_MID);
    lv_obj_remove_flag(ui_PnlVerLine, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PnlVerLine, lv_color_hex(0x313131), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PnlVerLine, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button3, ui_event_Button3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button4, ui_event_Button4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button5, ui_event_Button5, LV_EVENT_ALL, NULL);

}
