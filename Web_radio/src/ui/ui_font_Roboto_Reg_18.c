/*******************************************************************************
 * Size: 18 px
 * Bpp: 1
 * Opts: --bpp 1 --size 18 --font C:/Users/zuzelka.CML/Documents/GitHub/Web_radio/SquareLine Studio Project/assets/Roboto-Regular.ttf -o C:/Users/zuzelka.CML/Documents/GitHub/Web_radio/SquareLine Studio Project/assets\ui_font_Roboto_Reg_18.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_ROBOTO_REG_18
#define UI_FONT_ROBOTO_REG_18 1
#endif

#if UI_FONT_ROBOTO_REG_18

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xc3, 0xc0,

    /* U+0022 "\"" */
    0xb6, 0xd0,

    /* U+0023 "#" */
    0x9, 0x82, 0x41, 0x91, 0xff, 0x13, 0x4, 0x81,
    0x20, 0x48, 0xff, 0x88, 0x82, 0x60, 0x98, 0x24,
    0x0,

    /* U+0024 "$" */
    0x18, 0x18, 0x7e, 0xe7, 0xc3, 0xc3, 0xc0, 0x70,
    0x3c, 0xe, 0x7, 0xc3, 0xc3, 0xe7, 0x7e, 0x18,
    0x18,

    /* U+0025 "%" */
    0x70, 0x11, 0x2, 0x26, 0x44, 0x88, 0xa0, 0xec,
    0x1, 0x0, 0x6e, 0xa, 0x22, 0x44, 0xc8, 0x91,
    0x10, 0x1c,

    /* U+0026 "&" */
    0x3e, 0xc, 0x61, 0x8c, 0x31, 0x87, 0x60, 0x78,
    0xe, 0x3, 0x66, 0xc6, 0xd8, 0x73, 0x6, 0x31,
    0xc3, 0xec,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x8, 0xcc, 0xc6, 0x23, 0x18, 0xc6, 0x31, 0x8c,
    0x21, 0x8c, 0x30, 0x82,

    /* U+0029 ")" */
    0x86, 0x18, 0x43, 0x18, 0x63, 0x18, 0xc6, 0x31,
    0x88, 0xc6, 0x62, 0x20,

    /* U+002A "*" */
    0x18, 0x18, 0x99, 0xff, 0x38, 0x3c, 0x66, 0x0,

    /* U+002B "+" */
    0x18, 0xc, 0x6, 0x3, 0xf, 0xf8, 0xc0, 0x60,
    0x30, 0x18, 0x0,

    /* U+002C "," */
    0x6d, 0xe0,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x6, 0x8, 0x10, 0x60, 0x83, 0x6, 0x8, 0x30,
    0x40, 0x83, 0x4, 0x8, 0x30, 0x0,

    /* U+0030 "0" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+0031 "1" */
    0xd, 0xfe, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30,
    0xc3, 0xc,

    /* U+0032 "2" */
    0x3c, 0x66, 0xc3, 0xc3, 0x3, 0x7, 0x6, 0xc,
    0x18, 0x38, 0x70, 0x60, 0xff,

    /* U+0033 "3" */
    0x3c, 0x66, 0xc3, 0x3, 0x3, 0x6, 0x1c, 0x6,
    0x3, 0x3, 0xc3, 0x66, 0x3c,

    /* U+0034 "4" */
    0x3, 0x3, 0x83, 0xc1, 0x61, 0xb1, 0x98, 0x8c,
    0xc6, 0xc3, 0x7f, 0xc0, 0xc0, 0x60, 0x30,

    /* U+0035 "5" */
    0x7f, 0x40, 0xc0, 0xc0, 0xfc, 0xc6, 0x3, 0x3,
    0x3, 0x83, 0x83, 0xc6, 0x3c,

    /* U+0036 "6" */
    0x1c, 0x30, 0x60, 0x40, 0xdc, 0xe6, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+0037 "7" */
    0xff, 0x80, 0xc0, 0x40, 0x60, 0x20, 0x30, 0x10,
    0x18, 0xc, 0xc, 0x6, 0x6, 0x3, 0x0,

    /* U+0038 "8" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x66,
    0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+0039 "9" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x67, 0x3b,
    0x3, 0x3, 0x6, 0xc, 0x38,

    /* U+003A ":" */
    0xf0, 0x0, 0xf0,

    /* U+003B ";" */
    0x6c, 0x0, 0x0, 0x6d, 0xa4,

    /* U+003C "<" */
    0x2, 0x1c, 0xe7, 0xc, 0xf, 0x7, 0x83,

    /* U+003D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+003E ">" */
    0x80, 0xe0, 0x38, 0xf, 0x7, 0x3c, 0xf0, 0xc0,

    /* U+003F "?" */
    0x7d, 0x8f, 0x18, 0x30, 0x61, 0x86, 0x18, 0x30,
    0x0, 0x1, 0x83, 0x0,

    /* U+0040 "@" */
    0x7, 0xc0, 0x60, 0xc3, 0x1, 0x98, 0x2, 0x63,
    0xcb, 0x19, 0x1c, 0x44, 0x73, 0x11, 0xc8, 0x47,
    0x21, 0x1c, 0x8c, 0x73, 0x32, 0x67, 0x71, 0x80,
    0x3, 0x0, 0xe, 0x10, 0xf, 0xc0,

    /* U+0041 "A" */
    0x6, 0x0, 0x60, 0xf, 0x0, 0xb0, 0x19, 0x1,
    0x98, 0x11, 0x83, 0x8, 0x3f, 0xc2, 0x4, 0x60,
    0x64, 0x6, 0xc0, 0x20,

    /* U+0042 "B" */
    0xfe, 0x61, 0xb0, 0x78, 0x3c, 0x1e, 0x1b, 0xf9,
    0x86, 0xc1, 0xe0, 0xf0, 0x78, 0x6f, 0xe0,

    /* U+0043 "C" */
    0x1f, 0xc, 0x66, 0xf, 0x3, 0xc0, 0x30, 0xc,
    0x3, 0x0, 0xc0, 0x30, 0x36, 0xd, 0xc6, 0x1f,
    0x0,

    /* U+0044 "D" */
    0xfc, 0x63, 0x30, 0xd8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xe0, 0xf0, 0xd8, 0xcf, 0xc0,

    /* U+0045 "E" */
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0, 0xff,

    /* U+0046 "F" */
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0047 "G" */
    0x1f, 0x1c, 0x66, 0xf, 0x0, 0xc0, 0x30, 0xc,
    0x7f, 0x3, 0xc0, 0xf0, 0x36, 0xc, 0xc7, 0x1f,
    0x0,

    /* U+0048 "H" */
    0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc0, 0xf0, 0x3f,
    0xff, 0x3, 0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc0,
    0xc0,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xc0,

    /* U+004A "J" */
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
    0x3, 0x3, 0xc3, 0xe6, 0x3c,

    /* U+004B "K" */
    0xc1, 0xb0, 0xcc, 0x73, 0x18, 0xcc, 0x36, 0xf,
    0x83, 0xb0, 0xc6, 0x31, 0xcc, 0x33, 0x6, 0xc0,
    0xc0,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0, 0xff,

    /* U+004D "M" */
    0xe0, 0x3f, 0x1, 0xf8, 0xf, 0xe0, 0xfd, 0x5,
    0xec, 0x6f, 0x63, 0x79, 0x13, 0xcd, 0x9e, 0x28,
    0xf1, 0x47, 0x8e, 0x3c, 0x21, 0x80,

    /* U+004E "N" */
    0xc0, 0xf8, 0x3e, 0xf, 0xc3, 0xd8, 0xf6, 0x3c,
    0xcf, 0x13, 0xc6, 0xf0, 0xfc, 0x3f, 0x7, 0xc0,
    0xc0,

    /* U+004F "O" */
    0x1e, 0xc, 0xe6, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xf0, 0x36, 0x19, 0xc6, 0x1e,
    0x0,

    /* U+0050 "P" */
    0xfe, 0x61, 0xb0, 0x78, 0x3c, 0x1e, 0xf, 0xd,
    0xfc, 0xc0, 0x60, 0x30, 0x18, 0xc, 0x0,

    /* U+0051 "Q" */
    0x1e, 0xc, 0xc6, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xf0, 0x36, 0x19, 0xce, 0x1f,
    0x0, 0x60, 0xc,

    /* U+0052 "R" */
    0xfe, 0x30, 0xcc, 0x1b, 0x6, 0xc1, 0xb0, 0xcf,
    0xe3, 0x18, 0xc6, 0x30, 0xcc, 0x33, 0x6, 0xc1,
    0x80,

    /* U+0053 "S" */
    0x3e, 0x31, 0xb0, 0x78, 0x3e, 0x3, 0xc0, 0xf8,
    0x1e, 0x3, 0xe0, 0xf0, 0x6c, 0x63, 0xe0,

    /* U+0054 "T" */
    0xff, 0xc3, 0x0, 0xc0, 0x30, 0xc, 0x3, 0x0,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30, 0xc,
    0x0,

    /* U+0055 "U" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xe0, 0xf0, 0x6c, 0x63, 0xe0,

    /* U+0056 "V" */
    0xc0, 0x6c, 0xd, 0x81, 0x10, 0x63, 0xc, 0x61,
    0x4, 0x60, 0xc8, 0x1b, 0x1, 0x60, 0x38, 0x7,
    0x0, 0x60,

    /* U+0057 "W" */
    0x83, 0x5, 0x86, 0xb, 0xc, 0x36, 0x3c, 0x64,
    0x68, 0xc8, 0x91, 0x19, 0x36, 0x36, 0x6c, 0x28,
    0x58, 0x50, 0xa0, 0xa1, 0xc1, 0xc1, 0x83, 0x3,
    0x0,

    /* U+0058 "X" */
    0x60, 0xc4, 0x18, 0xc6, 0xc, 0xc1, 0xb0, 0x1c,
    0x1, 0x80, 0x70, 0x1b, 0x3, 0x30, 0xc6, 0x30,
    0x66, 0xe,

    /* U+0059 "Y" */
    0xc0, 0xf0, 0x36, 0x19, 0x86, 0x33, 0x4, 0x81,
    0xe0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30, 0xc,
    0x0,

    /* U+005A "Z" */
    0xff, 0x80, 0xc0, 0xc0, 0xc0, 0x60, 0x60, 0x60,
    0x30, 0x30, 0x18, 0x18, 0x18, 0xf, 0xf8,

    /* U+005B "[" */
    0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
    0xcf,

    /* U+005C "\\" */
    0x40, 0x81, 0x81, 0x3, 0x6, 0x4, 0xc, 0x18,
    0x10, 0x30, 0x20, 0x60, 0xc0, 0x80,

    /* U+005D "]" */
    0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x3f,

    /* U+005E "^" */
    0x20, 0xc7, 0x16, 0x5b, 0x28, 0xc0,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0xc6, 0x30,

    /* U+0061 "a" */
    0x3c, 0xe7, 0xc3, 0x3, 0x3f, 0xe3, 0xc3, 0xc3,
    0xe7, 0x7f,

    /* U+0062 "b" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xdc, 0xe6, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xe6, 0xfc,

    /* U+0063 "c" */
    0x3c, 0x62, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc3,
    0x62, 0x3c,

    /* U+0064 "d" */
    0x3, 0x3, 0x3, 0x3, 0x3f, 0x67, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0x67, 0x3f,

    /* U+0065 "e" */
    0x3c, 0x66, 0xc3, 0xc3, 0xff, 0xc0, 0xc0, 0xc0,
    0x63, 0x3e,

    /* U+0066 "f" */
    0x3d, 0x86, 0x18, 0xf9, 0x86, 0x18, 0x61, 0x86,
    0x18, 0x61, 0x80,

    /* U+0067 "g" */
    0x3f, 0x67, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0x67, 0x3f, 0x3, 0x43, 0x66, 0x3c,

    /* U+0068 "h" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xde, 0xe3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,

    /* U+0069 "i" */
    0xf3, 0xff, 0xff, 0xc0,

    /* U+006A "j" */
    0x33, 0x3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0xe0,

    /* U+006B "k" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc6, 0xce, 0xcc, 0xd8,
    0xf0, 0xf8, 0xcc, 0xc4, 0xc6, 0xc3,

    /* U+006C "l" */
    0xff, 0xff, 0xff, 0xf0,

    /* U+006D "m" */
    0xde, 0xfb, 0x9e, 0x7c, 0x30, 0xf0, 0xc3, 0xc3,
    0xf, 0xc, 0x3c, 0x30, 0xf0, 0xc3, 0xc3, 0xf,
    0xc, 0x30,

    /* U+006E "n" */
    0xde, 0xe3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3,

    /* U+006F "o" */
    0x3e, 0x31, 0xb0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0x63, 0x1f, 0x0,

    /* U+0070 "p" */
    0xfc, 0xe6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xe6, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0071 "q" */
    0x3f, 0x67, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0x67, 0x3f, 0x3, 0x3, 0x3, 0x3,

    /* U+0072 "r" */
    0xff, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x0,

    /* U+0073 "s" */
    0x3c, 0xc7, 0xc3, 0xe0, 0x7c, 0x3e, 0x7, 0xc3,
    0x63, 0x3c,

    /* U+0074 "t" */
    0x31, 0x8d, 0xf3, 0x18, 0xc6, 0x31, 0x8c, 0x61,
    0x80,

    /* U+0075 "u" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xe7, 0x7f,

    /* U+0076 "v" */
    0xc1, 0x43, 0x63, 0x62, 0x26, 0x36, 0x14, 0x1c,
    0x18, 0x8,

    /* U+0077 "w" */
    0x43, 0x1a, 0x18, 0xd9, 0xc4, 0xca, 0x22, 0x5b,
    0x16, 0x58, 0xf2, 0x87, 0x1c, 0x18, 0x60, 0xc2,
    0x0,

    /* U+0078 "x" */
    0x63, 0x31, 0xd, 0x83, 0x81, 0xc0, 0xe0, 0x50,
    0x6c, 0x63, 0x31, 0x80,

    /* U+0079 "y" */
    0x41, 0x31, 0x98, 0xc4, 0x43, 0x61, 0xb0, 0x50,
    0x38, 0x1c, 0x4, 0x2, 0x3, 0x1, 0x3, 0x0,

    /* U+007A "z" */
    0xfe, 0x6, 0xc, 0x18, 0x18, 0x30, 0x20, 0x60,
    0xc0, 0xff,

    /* U+007B "{" */
    0xc, 0x63, 0xc, 0x30, 0xc3, 0x18, 0xc1, 0x83,
    0xc, 0x30, 0xc3, 0x6, 0xc,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xff,

    /* U+007D "}" */
    0xc1, 0x83, 0xc, 0x30, 0xc3, 0x4, 0xc, 0x43,
    0xc, 0x30, 0xc3, 0x18, 0xc0,

    /* U+007E "~" */
    0x70, 0xf3, 0x3c, 0x38
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 71, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 74, .box_w = 2, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 92, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 7, .adv_w = 177, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 24, .adv_w = 162, .box_w = 8, .box_h = 17, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 41, .adv_w = 211, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 179, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 50, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 78, .adv_w = 98, .box_w = 5, .box_h = 19, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 90, .adv_w = 100, .box_w = 5, .box_h = 19, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 102, .adv_w = 124, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 110, .adv_w = 163, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 121, .adv_w = 57, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 123, .adv_w = 79, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 124, .adv_w = 76, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 119, .box_w = 7, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 139, .adv_w = 162, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 162, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 162, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 162, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 188, .adv_w = 162, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 203, .adv_w = 162, .box_w = 8, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 162, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 162, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 244, .adv_w = 162, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 162, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 70, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 61, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 278, .adv_w = 146, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 285, .adv_w = 158, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 290, .adv_w = 150, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 298, .adv_w = 136, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 259, .box_w = 14, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 340, .adv_w = 188, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 360, .adv_w = 179, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 375, .adv_w = 187, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 392, .adv_w = 189, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 164, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 159, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 433, .adv_w = 196, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 450, .adv_w = 205, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 78, .box_w = 2, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 471, .adv_w = 159, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 484, .adv_w = 181, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 155, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 514, .adv_w = 251, .box_w = 13, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 536, .adv_w = 205, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 198, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 570, .adv_w = 182, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 585, .adv_w = 198, .box_w = 10, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 604, .adv_w = 177, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 171, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 636, .adv_w = 172, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 653, .adv_w = 187, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 668, .adv_w = 183, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 686, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 711, .adv_w = 181, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 729, .adv_w = 173, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 746, .adv_w = 172, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 761, .adv_w = 76, .box_w = 4, .box_h = 18, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 770, .adv_w = 118, .box_w = 7, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 784, .adv_w = 76, .box_w = 4, .box_h = 18, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 793, .adv_w = 120, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 799, .adv_w = 130, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 800, .adv_w = 89, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 11},
    {.bitmap_index = 802, .adv_w = 157, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 812, .adv_w = 162, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 826, .adv_w = 151, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 836, .adv_w = 162, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 850, .adv_w = 153, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 860, .adv_w = 100, .box_w = 6, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 871, .adv_w = 162, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 885, .adv_w = 159, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 899, .adv_w = 70, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 903, .adv_w = 69, .box_w = 4, .box_h = 17, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 912, .adv_w = 146, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 926, .adv_w = 70, .box_w = 2, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 930, .adv_w = 252, .box_w = 14, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 948, .adv_w = 159, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 958, .adv_w = 164, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 970, .adv_w = 162, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 984, .adv_w = 164, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 998, .adv_w = 97, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1005, .adv_w = 149, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1015, .adv_w = 94, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1024, .adv_w = 159, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1034, .adv_w = 140, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1044, .adv_w = 216, .box_w = 13, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1061, .adv_w = 143, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1073, .adv_w = 136, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 1089, .adv_w = 143, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1099, .adv_w = 97, .box_w = 6, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1112, .adv_w = 70, .box_w = 2, .box_h = 16, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 1116, .adv_w = 97, .box_w = 6, .box_h = 17, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1129, .adv_w = 196, .box_w = 10, .box_h = 3, .ofs_x = 1, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    1, 53,
    3, 3,
    3, 8,
    3, 34,
    3, 66,
    3, 68,
    3, 69,
    3, 70,
    3, 72,
    3, 78,
    3, 79,
    3, 80,
    3, 81,
    3, 82,
    3, 84,
    3, 88,
    8, 3,
    8, 8,
    8, 34,
    8, 66,
    8, 68,
    8, 69,
    8, 70,
    8, 72,
    8, 78,
    8, 79,
    8, 80,
    8, 81,
    8, 82,
    8, 84,
    8, 88,
    9, 55,
    9, 56,
    9, 58,
    13, 3,
    13, 8,
    15, 3,
    15, 8,
    16, 16,
    34, 3,
    34, 8,
    34, 32,
    34, 36,
    34, 40,
    34, 48,
    34, 50,
    34, 53,
    34, 54,
    34, 55,
    34, 56,
    34, 58,
    34, 80,
    34, 85,
    34, 86,
    34, 87,
    34, 88,
    34, 90,
    34, 91,
    35, 53,
    35, 55,
    35, 58,
    36, 10,
    36, 53,
    36, 62,
    36, 94,
    37, 13,
    37, 15,
    37, 34,
    37, 53,
    37, 55,
    37, 57,
    37, 58,
    37, 59,
    38, 53,
    38, 68,
    38, 69,
    38, 70,
    38, 71,
    38, 72,
    38, 80,
    38, 82,
    38, 86,
    38, 87,
    38, 88,
    38, 90,
    39, 13,
    39, 15,
    39, 34,
    39, 43,
    39, 53,
    39, 66,
    39, 68,
    39, 69,
    39, 70,
    39, 72,
    39, 80,
    39, 82,
    39, 83,
    39, 86,
    39, 87,
    39, 90,
    41, 34,
    41, 53,
    41, 57,
    41, 58,
    42, 34,
    42, 53,
    42, 57,
    42, 58,
    43, 34,
    44, 14,
    44, 36,
    44, 40,
    44, 48,
    44, 50,
    44, 68,
    44, 69,
    44, 70,
    44, 72,
    44, 78,
    44, 79,
    44, 80,
    44, 81,
    44, 82,
    44, 86,
    44, 87,
    44, 88,
    44, 90,
    45, 3,
    45, 8,
    45, 34,
    45, 36,
    45, 40,
    45, 48,
    45, 50,
    45, 53,
    45, 54,
    45, 55,
    45, 56,
    45, 58,
    45, 86,
    45, 87,
    45, 88,
    45, 90,
    46, 34,
    46, 53,
    46, 57,
    46, 58,
    47, 34,
    47, 53,
    47, 57,
    47, 58,
    48, 13,
    48, 15,
    48, 34,
    48, 53,
    48, 55,
    48, 57,
    48, 58,
    48, 59,
    49, 13,
    49, 15,
    49, 34,
    49, 43,
    49, 57,
    49, 59,
    49, 66,
    49, 68,
    49, 69,
    49, 70,
    49, 72,
    49, 80,
    49, 82,
    49, 85,
    49, 87,
    49, 90,
    50, 53,
    50, 55,
    50, 56,
    50, 58,
    51, 53,
    51, 55,
    51, 58,
    53, 1,
    53, 13,
    53, 14,
    53, 15,
    53, 34,
    53, 36,
    53, 40,
    53, 43,
    53, 48,
    53, 50,
    53, 52,
    53, 53,
    53, 55,
    53, 56,
    53, 58,
    53, 66,
    53, 68,
    53, 69,
    53, 70,
    53, 72,
    53, 78,
    53, 79,
    53, 80,
    53, 81,
    53, 82,
    53, 83,
    53, 84,
    53, 86,
    53, 87,
    53, 88,
    53, 89,
    53, 90,
    53, 91,
    54, 34,
    55, 10,
    55, 13,
    55, 14,
    55, 15,
    55, 34,
    55, 36,
    55, 40,
    55, 48,
    55, 50,
    55, 62,
    55, 66,
    55, 68,
    55, 69,
    55, 70,
    55, 72,
    55, 80,
    55, 82,
    55, 83,
    55, 86,
    55, 87,
    55, 90,
    55, 94,
    56, 10,
    56, 13,
    56, 14,
    56, 15,
    56, 34,
    56, 53,
    56, 62,
    56, 66,
    56, 68,
    56, 69,
    56, 70,
    56, 72,
    56, 80,
    56, 82,
    56, 83,
    56, 86,
    56, 94,
    57, 14,
    57, 36,
    57, 40,
    57, 48,
    57, 50,
    57, 55,
    57, 68,
    57, 69,
    57, 70,
    57, 72,
    57, 80,
    57, 82,
    57, 86,
    57, 87,
    57, 90,
    58, 7,
    58, 10,
    58, 11,
    58, 13,
    58, 14,
    58, 15,
    58, 34,
    58, 36,
    58, 40,
    58, 43,
    58, 48,
    58, 50,
    58, 52,
    58, 53,
    58, 54,
    58, 55,
    58, 56,
    58, 57,
    58, 58,
    58, 62,
    58, 66,
    58, 68,
    58, 69,
    58, 70,
    58, 71,
    58, 72,
    58, 78,
    58, 79,
    58, 80,
    58, 81,
    58, 82,
    58, 83,
    58, 84,
    58, 85,
    58, 86,
    58, 87,
    58, 89,
    58, 90,
    58, 91,
    58, 94,
    59, 34,
    59, 36,
    59, 40,
    59, 48,
    59, 50,
    59, 68,
    59, 69,
    59, 70,
    59, 72,
    59, 80,
    59, 82,
    59, 86,
    59, 87,
    59, 88,
    59, 90,
    60, 43,
    60, 54,
    66, 3,
    66, 8,
    66, 87,
    66, 90,
    67, 3,
    67, 8,
    67, 87,
    67, 89,
    67, 90,
    67, 91,
    68, 3,
    68, 8,
    70, 3,
    70, 8,
    70, 87,
    70, 90,
    71, 3,
    71, 8,
    71, 10,
    71, 62,
    71, 68,
    71, 69,
    71, 70,
    71, 72,
    71, 82,
    71, 94,
    73, 3,
    73, 8,
    76, 68,
    76, 69,
    76, 70,
    76, 72,
    76, 82,
    78, 3,
    78, 8,
    79, 3,
    79, 8,
    80, 3,
    80, 8,
    80, 87,
    80, 89,
    80, 90,
    80, 91,
    81, 3,
    81, 8,
    81, 87,
    81, 89,
    81, 90,
    81, 91,
    83, 3,
    83, 8,
    83, 13,
    83, 15,
    83, 66,
    83, 68,
    83, 69,
    83, 70,
    83, 71,
    83, 72,
    83, 80,
    83, 82,
    83, 85,
    83, 87,
    83, 88,
    83, 90,
    85, 80,
    87, 3,
    87, 8,
    87, 13,
    87, 15,
    87, 66,
    87, 68,
    87, 69,
    87, 70,
    87, 71,
    87, 72,
    87, 80,
    87, 82,
    88, 13,
    88, 15,
    89, 68,
    89, 69,
    89, 70,
    89, 72,
    89, 80,
    89, 82,
    90, 3,
    90, 8,
    90, 13,
    90, 15,
    90, 66,
    90, 68,
    90, 69,
    90, 70,
    90, 71,
    90, 72,
    90, 80,
    90, 82,
    91, 68,
    91, 69,
    91, 70,
    91, 72,
    91, 80,
    91, 82,
    92, 43,
    92, 54
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -6, -15, -15, -17, -7, -8, -8, -8,
    -8, -3, -3, -9, -3, -8, -11, 2,
    -15, -15, -17, -7, -8, -8, -8, -8,
    -3, -3, -9, -3, -8, -11, 2, 3,
    3, 3, -24, -24, -24, -24, -31, -17,
    -17, -9, -2, -2, -2, -2, -18, -2,
    -12, -10, -13, -2, -2, -2, -7, -5,
    -7, 2, -4, -3, -8, -4, -4, -2,
    -2, -14, -14, -3, -4, -3, -3, -6,
    -3, 3, -3, -3, -3, -3, -3, -3,
    -3, -2, -4, -3, -4, -33, -33, -24,
    -37, 3, -5, -3, -3, -3, -3, -3,
    -3, -4, -3, -3, -3, 3, -4, 2,
    -4, 3, -4, 2, -4, -3, -9, -4,
    -4, -4, -4, -4, -4, -4, -4, -3,
    -3, -4, -3, -4, -3, -6, -9, -6,
    -47, -47, 3, -9, -9, -9, -9, -39,
    -8, -25, -20, -34, -6, -19, -13, -19,
    3, -4, 2, -4, 3, -4, 2, -4,
    -14, -14, -3, -4, -3, -3, -6, -3,
    -46, -46, -19, -28, -4, -4, -2, -2,
    -2, -2, -2, -2, -2, 2, 2, 2,
    -6, -4, -3, -5, -11, -3, -7, -6,
    -31, -33, -31, -11, -4, -4, -34, -4,
    -4, -2, 2, 2, 2, 2, -16, -14,
    -14, -14, -14, -15, -15, -14, -15, -14,
    -11, -16, -13, -10, -8, -11, -10, -8,
    -3, 3, -32, -5, -32, -11, -2, -2,
    -2, -2, 2, -6, -6, -6, -6, -6,
    -6, -6, -4, -4, -2, -2, 3, 2,
    -17, -8, -17, -6, 2, 2, -5, -4,
    -4, -4, -4, -4, -4, -3, -3, 2,
    -6, -4, -4, -4, -4, 2, -4, -4,
    -4, -4, -3, -4, -3, -4, -4, -4,
    3, -7, -30, -7, -30, -13, -4, -4,
    -13, -4, -4, -2, 2, -13, 3, 2,
    2, 3, 3, -10, -9, -9, -9, -3,
    -9, -6, -6, -9, -6, -9, -6, -8,
    -3, -5, -3, -3, -3, -4, 3, 2,
    -4, -4, -4, -4, -3, -3, -3, -3,
    -3, -3, -3, -4, -4, -4, -3, -3,
    -9, -9, -2, -2, -4, -4, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2,
    2, 2, 3, 3, -3, -3, -3, -3,
    -3, 3, -15, -15, -3, -3, -3, -3,
    -3, -15, -15, -15, -15, -19, -19, -2,
    -3, -2, -2, -4, -4, -2, -2, -2,
    -2, 2, 2, -17, -17, -6, -3, -3,
    -3, 2, -3, -3, -3, 7, 3, 2,
    3, -3, 2, 2, -15, -15, -2, -2,
    -2, -2, 2, -2, -2, -2, -17, -17,
    -3, -3, -3, -3, -3, -3, 2, 2,
    -15, -15, -2, -2, -2, -2, 2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2,
    -3, -3
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 434,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Roboto_Reg_18 = {
#else
lv_font_t ui_font_Roboto_Reg_18 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 19,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_ROBOTO_REG_18*/

