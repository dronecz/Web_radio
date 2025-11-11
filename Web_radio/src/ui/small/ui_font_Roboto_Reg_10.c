/*******************************************************************************
 * Size: 10 px
 * Bpp: 1
 * Opts: --bpp 1 --size 10 --font C:/Users/dronecz/Documents/GitHub/Web_radio/SLS_project_small/assets/Roboto-Regular.ttf -o C:/Users/dronecz/Documents/GitHub/Web_radio/SLS_project_small/assets\ui_font_Roboto_Reg_10.c --format lvgl -r 0x20-0xff --symbols ěščřžĚŠČŘŽ´ --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_ROBOTO_REG_10
#define UI_FONT_ROBOTO_REG_10 1
#endif

#if UI_FONT_ROBOTO_REG_10

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xf9,

    /* U+0022 "\"" */
    0xf0,

    /* U+0023 "#" */
    0x52, 0xbe, 0xa5, 0x7e, 0x94,

    /* U+0024 "$" */
    0x23, 0xa3, 0x14, 0x18, 0x31, 0x71, 0x0,

    /* U+0025 "%" */
    0xe1, 0x52, 0xa7, 0x82, 0xc6, 0x54, 0x86,

    /* U+0026 "&" */
    0x72, 0x94, 0xc6, 0x4e, 0x4f,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x2a, 0x49, 0x24, 0x89, 0x0,

    /* U+0029 ")" */
    0x89, 0x12, 0x49, 0x29, 0x40,

    /* U+002A "*" */
    0x25, 0x1c, 0xa1, 0x0,

    /* U+002B "+" */
    0x21, 0x9, 0xf2, 0x10,

    /* U+002C "," */
    0xc0,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x12, 0x22, 0x44, 0x48, 0x80,

    /* U+0030 "0" */
    0x74, 0x63, 0x18, 0xc6, 0x2e,

    /* U+0031 "1" */
    0x3c, 0x92, 0x49,

    /* U+0032 "2" */
    0x69, 0x91, 0x24, 0xcf,

    /* U+0033 "3" */
    0x69, 0x16, 0x11, 0x96,

    /* U+0034 "4" */
    0x11, 0x94, 0xa9, 0x7c, 0x42,

    /* U+0035 "5" */
    0x7a, 0x10, 0xe0, 0x85, 0x2e,

    /* U+0036 "6" */
    0x32, 0x21, 0xe8, 0xc6, 0x2e,

    /* U+0037 "7" */
    0xf8, 0x44, 0x22, 0x10, 0x88,

    /* U+0038 "8" */
    0x69, 0x96, 0x99, 0x96,

    /* U+0039 "9" */
    0x74, 0x63, 0x17, 0x84, 0x4c,

    /* U+003A ":" */
    0x84,

    /* U+003B ";" */
    0x8e,

    /* U+003C "<" */
    0x16, 0x86, 0x10,

    /* U+003D "=" */
    0xf0, 0xf0,

    /* U+003E ">" */
    0x86, 0x16, 0x80,

    /* U+003F "?" */
    0x72, 0x42, 0x33, 0x10, 0x4,

    /* U+0040 "@" */
    0x3c, 0x42, 0x59, 0xa9, 0xa9, 0xa9, 0xa9, 0xb6,
    0x40, 0x38,

    /* U+0041 "A" */
    0x10, 0xc2, 0x8a, 0x49, 0xf4, 0x61,

    /* U+0042 "B" */
    0xf4, 0x63, 0xe8, 0xc6, 0x3e,

    /* U+0043 "C" */
    0x73, 0x28, 0x60, 0x82, 0x1c, 0x9c,

    /* U+0044 "D" */
    0xf4, 0xa3, 0x18, 0xc6, 0x5e,

    /* U+0045 "E" */
    0xfc, 0x21, 0xe8, 0x42, 0x1f,

    /* U+0046 "F" */
    0xf8, 0x88, 0xf8, 0x88,

    /* U+0047 "G" */
    0x7b, 0x38, 0x20, 0x9e, 0x14, 0x5e,

    /* U+0048 "H" */
    0x8c, 0x63, 0xf8, 0xc6, 0x31,

    /* U+0049 "I" */
    0xff,

    /* U+004A "J" */
    0x8, 0x42, 0x10, 0x86, 0x2e,

    /* U+004B "K" */
    0x8a, 0x4a, 0x38, 0xe2, 0x49, 0x22,

    /* U+004C "L" */
    0x88, 0x88, 0x88, 0x8f,

    /* U+004D "M" */
    0x83, 0x8f, 0x1e, 0x5a, 0xb5, 0x6c, 0xc9,

    /* U+004E "N" */
    0x87, 0x1e, 0x69, 0x96, 0x78, 0xe1,

    /* U+004F "O" */
    0x79, 0x28, 0x61, 0x86, 0x1c, 0xde,

    /* U+0050 "P" */
    0xf4, 0x63, 0x1f, 0x42, 0x10,

    /* U+0051 "Q" */
    0x79, 0x28, 0x61, 0x86, 0x1c, 0x9e, 0xc,

    /* U+0052 "R" */
    0xf4, 0x63, 0x1f, 0x4a, 0x71,

    /* U+0053 "S" */
    0x74, 0x60, 0x83, 0x6, 0x2e,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x10, 0x84,

    /* U+0055 "U" */
    0x8c, 0x63, 0x18, 0xc6, 0x2e,

    /* U+0056 "V" */
    0x85, 0x14, 0x92, 0x28, 0xc3, 0x4,

    /* U+0057 "W" */
    0x88, 0xa4, 0x95, 0x4a, 0xa5, 0x52, 0xb0, 0x88,
    0x44,

    /* U+0058 "X" */
    0x45, 0x23, 0x8c, 0x30, 0xe4, 0x91,

    /* U+0059 "Y" */
    0x8c, 0x54, 0xa2, 0x10, 0x84,

    /* U+005A "Z" */
    0xf8, 0x44, 0x42, 0x22, 0x1f,

    /* U+005B "[" */
    0xea, 0xaa, 0xac,

    /* U+005C "\\" */
    0x84, 0x44, 0x22, 0x21, 0x10,

    /* U+005D "]" */
    0xd5, 0x55, 0x5c,

    /* U+005E "^" */
    0x26, 0x69,

    /* U+005F "_" */
    0xf8,

    /* U+0060 "`" */
    0x90,

    /* U+0061 "a" */
    0x79, 0x79, 0x9f,

    /* U+0062 "b" */
    0x84, 0x3d, 0x18, 0xc6, 0x3e,

    /* U+0063 "c" */
    0x74, 0xa1, 0x9, 0x38,

    /* U+0064 "d" */
    0x8, 0x5f, 0x18, 0xc6, 0x2f,

    /* U+0065 "e" */
    0x74, 0x7f, 0xc, 0xb8,

    /* U+0066 "f" */
    0x29, 0x74, 0x92, 0x40,

    /* U+0067 "g" */
    0x7c, 0x63, 0x18, 0xbc, 0x2e,

    /* U+0068 "h" */
    0x88, 0xf9, 0x99, 0x99,

    /* U+0069 "i" */
    0xbf,

    /* U+006A "j" */
    0x45, 0x55, 0x70,

    /* U+006B "k" */
    0x88, 0x9a, 0xca, 0xa9,

    /* U+006C "l" */
    0xff,

    /* U+006D "m" */
    0xef, 0x26, 0x4c, 0x99, 0x32, 0x40,

    /* U+006E "n" */
    0xf9, 0x99, 0x99,

    /* U+006F "o" */
    0x74, 0x63, 0x18, 0xb8,

    /* U+0070 "p" */
    0xf4, 0x63, 0x18, 0xfa, 0x10,

    /* U+0071 "q" */
    0x7c, 0x63, 0x18, 0xbc, 0x21,

    /* U+0072 "r" */
    0xf2, 0x49, 0x0,

    /* U+0073 "s" */
    0x69, 0xc3, 0x96,

    /* U+0074 "t" */
    0x4b, 0xa4, 0x93,

    /* U+0075 "u" */
    0x99, 0x99, 0x9f,

    /* U+0076 "v" */
    0x8a, 0x94, 0xa2, 0x10,

    /* U+0077 "w" */
    0x93, 0x35, 0xab, 0x66, 0x44, 0x80,

    /* U+0078 "x" */
    0x52, 0x88, 0x45, 0x68,

    /* U+0079 "y" */
    0x92, 0x94, 0xc2, 0x10, 0x98,

    /* U+007A "z" */
    0xf1, 0x24, 0x8f,

    /* U+007B "{" */
    0x29, 0x25, 0x12, 0x48, 0x80,

    /* U+007C "|" */
    0xff, 0xc0,

    /* U+007D "}" */
    0x89, 0x24, 0x52, 0x4a, 0x0,

    /* U+007E "~" */
    0xea, 0x60,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xbe,

    /* U+00A2 "¢" */
    0x23, 0xb5, 0x8, 0x43, 0x2e, 0x20,

    /* U+00A3 "£" */
    0x32, 0x50, 0x8e, 0x21, 0x1f,

    /* U+00A4 "¤" */
    0xb5, 0x28, 0x61, 0x85, 0x2b, 0xc0,

    /* U+00A5 "¥" */
    0x8c, 0x54, 0xaf, 0xfc, 0x84,

    /* U+00A6 "¦" */
    0xf7, 0x80,

    /* U+00A7 "§" */
    0x74, 0x62, 0xcb, 0xc7, 0x26, 0x8c, 0x5c,

    /* U+00A8 "¨" */
    0xd0,

    /* U+00A9 "©" */
    0x38, 0xba, 0xad, 0x1a, 0xb7, 0x51, 0x1c,

    /* U+00AA "ª" */
    0xf7, 0xf0,

    /* U+00AB "«" */
    0x56, 0x65,

    /* U+00AC "¬" */
    0xf1, 0x10,

    /* U+00AD "­" */
    0xe0,

    /* U+00AE "®" */
    0x38, 0xfa, 0xad, 0x9a, 0xb5, 0x51, 0x1c,

    /* U+00AF "¯" */
    0xe0,

    /* U+00B0 "°" */
    0xf7, 0x80,

    /* U+00B1 "±" */
    0x21, 0x3e, 0x42, 0x3, 0xe0,

    /* U+00B2 "²" */
    0xe5, 0x70,

    /* U+00B3 "³" */
    0xe8, 0xf0,

    /* U+00B4 "´" */
    0x60,

    /* U+00B5 "µ" */
    0x99, 0x99, 0x9f, 0x88,

    /* U+00B6 "¶" */
    0x7f, 0xff, 0x71, 0x11,

    /* U+00B7 "·" */
    0x80,

    /* U+00B8 "¸" */
    0x1c,

    /* U+00B9 "¹" */
    0x55,

    /* U+00BA "º" */
    0x69, 0x96,

    /* U+00BB "»" */
    0x65, 0x56,

    /* U+00BC "¼" */
    0x1, 0x91, 0x42, 0x86, 0x47, 0x97, 0x82,

    /* U+00BD "½" */
    0x1, 0x91, 0x22, 0x86, 0xe4, 0x51, 0x7,

    /* U+00BE "¾" */
    0xe0, 0x51, 0x21, 0x8f, 0x45, 0x93, 0x82,

    /* U+00BF "¿" */
    0x20, 0x44, 0xc8, 0x96,

    /* U+00C0 "À" */
    0x20, 0x41, 0xc, 0x28, 0xa4, 0x9f, 0x46, 0x10,

    /* U+00C1 "Á" */
    0x10, 0x41, 0xc, 0x28, 0xa4, 0x9f, 0x46, 0x10,

    /* U+00C2 "Â" */
    0x10, 0xa1, 0xc, 0x28, 0xa4, 0x9f, 0x46, 0x10,

    /* U+00C3 "Ã" */
    0x29, 0x61, 0xc, 0x28, 0xa4, 0x9f, 0x46, 0x10,

    /* U+00C4 "Ä" */
    0x28, 0x1, 0xc, 0x28, 0xa4, 0x9f, 0x46, 0x10,

    /* U+00C5 "Å" */
    0x30, 0xc3, 0xc, 0x30, 0xc4, 0x92, 0x7a, 0x18,
    0x40,

    /* U+00C6 "Æ" */
    0xf, 0x83, 0x1, 0x40, 0x5e, 0x24, 0x1f, 0x4,
    0x42, 0x1f,

    /* U+00C7 "Ç" */
    0x7b, 0x28, 0x60, 0x82, 0x1c, 0x9e, 0x10, 0xc0,

    /* U+00C8 "È" */
    0x41, 0x3f, 0x8, 0x7a, 0x10, 0x87, 0xc0,

    /* U+00C9 "É" */
    0x20, 0x3f, 0x8, 0x7a, 0x10, 0x87, 0xc0,

    /* U+00CA "Ê" */
    0x22, 0xbf, 0x8, 0x7a, 0x10, 0x87, 0xc0,

    /* U+00CB "Ë" */
    0xd0, 0x3f, 0x8, 0x7a, 0x10, 0x87, 0xc0,

    /* U+00CC "Ì" */
    0x95, 0x55, 0x50,

    /* U+00CD "Í" */
    0x6a, 0xaa, 0xa0,

    /* U+00CE "Î" */
    0x55, 0x24, 0x92, 0x48,

    /* U+00CF "Ï" */
    0xa1, 0x24, 0x92, 0x48,

    /* U+00D0 "Ð" */
    0x79, 0x24, 0x79, 0x45, 0x14, 0x9e,

    /* U+00D1 "Ñ" */
    0x69, 0x68, 0x71, 0xe6, 0x99, 0x67, 0x8e, 0x10,

    /* U+00D2 "Ò" */
    0x20, 0x47, 0x92, 0x86, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00D3 "Ó" */
    0x10, 0x87, 0x92, 0x86, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00D4 "Ô" */
    0x31, 0x27, 0x92, 0x86, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00D5 "Õ" */
    0x78, 0x7, 0x92, 0x86, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00D6 "Ö" */
    0x50, 0x7, 0x92, 0x86, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00D7 "×" */
    0xca, 0x88, 0xa0, 0x80,

    /* U+00D8 "Ø" */
    0x7f, 0x28, 0xe5, 0xa6, 0x94, 0xde, 0x80,

    /* U+00D9 "Ù" */
    0x41, 0x23, 0x18, 0xc6, 0x31, 0x8b, 0x80,

    /* U+00DA "Ú" */
    0x11, 0x23, 0x18, 0xc6, 0x31, 0x8b, 0x80,

    /* U+00DB "Û" */
    0x22, 0xa3, 0x18, 0xc6, 0x31, 0x8b, 0x80,

    /* U+00DC "Ü" */
    0x50, 0x23, 0x18, 0xc6, 0x31, 0x8b, 0x80,

    /* U+00DD "Ý" */
    0x11, 0x23, 0x15, 0x28, 0x84, 0x21, 0x0,

    /* U+00DE "Þ" */
    0x84, 0x3d, 0x18, 0xfa, 0x10,

    /* U+00DF "ß" */
    0x74, 0xa5, 0x4b, 0x46, 0x3f,

    /* U+00E0 "à" */
    0x40, 0x79, 0x79, 0x9f,

    /* U+00E1 "á" */
    0x20, 0x79, 0x79, 0x9f,

    /* U+00E2 "â" */
    0x24, 0x79, 0x79, 0x9f,

    /* U+00E3 "ã" */
    0x70, 0x79, 0x79, 0x9f,

    /* U+00E4 "ä" */
    0x50, 0x79, 0x79, 0x9f,

    /* U+00E5 "å" */
    0x26, 0x27, 0x97, 0x99, 0xf0,

    /* U+00E6 "æ" */
    0xee, 0x91, 0x7f, 0x90, 0x99, 0xee,

    /* U+00E7 "ç" */
    0x74, 0xa1, 0x9, 0x38, 0x84,

    /* U+00E8 "è" */
    0x60, 0x1d, 0x1f, 0xc3, 0x2e,

    /* U+00E9 "é" */
    0x10, 0x1d, 0x1f, 0xc3, 0x2e,

    /* U+00EA "ê" */
    0x22, 0x9d, 0x1f, 0xc3, 0x2e,

    /* U+00EB "ë" */
    0x50, 0x1d, 0x1f, 0xc3, 0x2e,

    /* U+00EC "ì" */
    0x95, 0x55,

    /* U+00ED "í" */
    0x6a, 0xaa,

    /* U+00EE "î" */
    0x45, 0x24, 0x92,

    /* U+00EF "ï" */
    0xa1, 0x24, 0x92,

    /* U+00F0 "ð" */
    0x61, 0x84, 0xfc, 0xc6, 0x3b, 0x70,

    /* U+00F1 "ñ" */
    0x70, 0xf9, 0x99, 0x99,

    /* U+00F2 "ò" */
    0x40, 0x1d, 0x18, 0xc6, 0x2e,

    /* U+00F3 "ó" */
    0x11, 0x1d, 0x18, 0xc6, 0x2e,

    /* U+00F4 "ô" */
    0x22, 0x1d, 0x18, 0xc6, 0x2e,

    /* U+00F5 "õ" */
    0x70, 0x1d, 0x18, 0xc6, 0x2e,

    /* U+00F6 "ö" */
    0x50, 0x1d, 0x18, 0xc6, 0x2e,

    /* U+00F7 "÷" */
    0x20, 0x1, 0xf0, 0x10,

    /* U+00F8 "ø" */
    0xb, 0xa7, 0x5a, 0xe5, 0xc0,

    /* U+00F9 "ù" */
    0x40, 0x99, 0x99, 0x9f,

    /* U+00FA "ú" */
    0x20, 0x99, 0x99, 0x9f,

    /* U+00FB "û" */
    0x60, 0x99, 0x99, 0x9f,

    /* U+00FC "ü" */
    0xa0, 0x99, 0x99, 0x9f,

    /* U+00FD "ý" */
    0x11, 0x24, 0xa5, 0x30, 0x84, 0x26, 0x0,

    /* U+00FE "þ" */
    0x84, 0x3d, 0x18, 0xc6, 0x3e, 0x84, 0x0,

    /* U+00FF "ÿ" */
    0x50, 0x24, 0xa5, 0x30, 0x84, 0x26, 0x0,

    /* U+010C "Č" */
    0x40, 0xc0, 0x1c, 0xca, 0x18, 0x20, 0x87, 0x27,
    0x0,

    /* U+010D "č" */
    0x60, 0x1d, 0x28, 0x42, 0x4e,

    /* U+011A "Ě" */
    0x61, 0x3f, 0x8, 0x7a, 0x10, 0x87, 0xc0,

    /* U+011B "ě" */
    0x51, 0x1d, 0x1f, 0xc3, 0x2e,

    /* U+0158 "Ř" */
    0x61, 0x3d, 0x18, 0xc7, 0xd2, 0x9c, 0x40,

    /* U+0159 "ř" */
    0xc3, 0xc9, 0x24,

    /* U+0160 "Š" */
    0x51, 0x1d, 0x18, 0x20, 0xc1, 0x8b, 0x80,

    /* U+0161 "š" */
    0x60, 0x69, 0xc3, 0x96,

    /* U+017D "Ž" */
    0x51, 0x3e, 0x11, 0x10, 0x88, 0x87, 0xc0,

    /* U+017E "ž" */
    0x60, 0xf1, 0x24, 0x8f
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 40, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 41, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 51, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 3, .adv_w = 99, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 8, .adv_w = 90, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 15, .adv_w = 117, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 99, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 27, .adv_w = 28, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 28, .adv_w = 55, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 33, .adv_w = 56, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 38, .adv_w = 69, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 42, .adv_w = 91, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 46, .adv_w = 31, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 47, .adv_w = 44, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 48, .adv_w = 42, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 66, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 54, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 90, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 90, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 90, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 70, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 85, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 90, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 99, .adv_w = 39, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 34, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 101, .adv_w = 81, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 104, .adv_w = 88, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 106, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 109, .adv_w = 76, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 144, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 124, .adv_w = 104, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 130, .adv_w = 100, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 104, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 105, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 88, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 109, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 114, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 44, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 167, .adv_w = 88, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 100, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 86, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 140, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 114, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 101, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 213, .adv_w = 99, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 95, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 223, .adv_w = 95, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 228, .adv_w = 104, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 102, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 239, .adv_w = 142, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 100, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 42, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 267, .adv_w = 66, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 272, .adv_w = 42, .box_w = 2, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 275, .adv_w = 67, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 277, .adv_w = 72, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 278, .adv_w = 49, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 279, .adv_w = 87, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 282, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 84, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 85, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 56, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 304, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 309, .adv_w = 88, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 313, .adv_w = 39, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 38, .box_w = 2, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 317, .adv_w = 81, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 321, .adv_w = 39, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 140, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 88, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 91, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 340, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 345, .adv_w = 54, .box_w = 3, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 83, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 351, .adv_w = 52, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 88, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 78, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 361, .adv_w = 120, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 79, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 371, .adv_w = 76, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 376, .adv_w = 79, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 54, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 384, .adv_w = 39, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 386, .adv_w = 54, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 391, .adv_w = 109, .box_w = 6, .box_h = 2, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 393, .adv_w = 40, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 39, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 395, .adv_w = 88, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 401, .adv_w = 93, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 406, .adv_w = 114, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 412, .adv_w = 97, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 417, .adv_w = 38, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 419, .adv_w = 98, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 426, .adv_w = 67, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 427, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 71, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 436, .adv_w = 75, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 438, .adv_w = 89, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 440, .adv_w = 44, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 441, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 73, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 449, .adv_w = 60, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 451, .adv_w = 85, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 456, .adv_w = 59, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 458, .adv_w = 59, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 460, .adv_w = 50, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 461, .adv_w = 91, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 465, .adv_w = 78, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 469, .adv_w = 42, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 470, .adv_w = 40, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 471, .adv_w = 59, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 472, .adv_w = 73, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 474, .adv_w = 75, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 476, .adv_w = 117, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 124, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 124, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 497, .adv_w = 76, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 501, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 509, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 517, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 525, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 104, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 550, .adv_w = 150, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 568, .adv_w = 91, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 91, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 91, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 589, .adv_w = 91, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 44, .box_w = 2, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 599, .adv_w = 44, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 602, .adv_w = 44, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 606, .adv_w = 44, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 610, .adv_w = 107, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 616, .adv_w = 114, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 624, .adv_w = 110, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 632, .adv_w = 110, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 110, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 648, .adv_w = 110, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 656, .adv_w = 110, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 664, .adv_w = 85, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 668, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 675, .adv_w = 104, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 682, .adv_w = 104, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 689, .adv_w = 104, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 696, .adv_w = 104, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 703, .adv_w = 96, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 710, .adv_w = 95, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 715, .adv_w = 95, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 720, .adv_w = 87, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 724, .adv_w = 87, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 728, .adv_w = 87, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 732, .adv_w = 87, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 736, .adv_w = 87, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 740, .adv_w = 87, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 745, .adv_w = 135, .box_w = 8, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 751, .adv_w = 84, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 756, .adv_w = 85, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 761, .adv_w = 85, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 766, .adv_w = 85, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 771, .adv_w = 85, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 776, .adv_w = 40, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 778, .adv_w = 40, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 780, .adv_w = 40, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 783, .adv_w = 40, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 786, .adv_w = 94, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 792, .adv_w = 88, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 796, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 801, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 806, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 811, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 816, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 821, .adv_w = 91, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 825, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 830, .adv_w = 88, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 834, .adv_w = 88, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 838, .adv_w = 88, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 842, .adv_w = 88, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 846, .adv_w = 76, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 853, .adv_w = 92, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 860, .adv_w = 76, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 867, .adv_w = 104, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 876, .adv_w = 84, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 881, .adv_w = 91, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 888, .adv_w = 85, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 893, .adv_w = 99, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 900, .adv_w = 54, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 903, .adv_w = 95, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 910, .adv_w = 83, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 914, .adv_w = 96, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 921, .adv_w = 79, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_2[] = {
    0x0, 0x1, 0xe, 0xf, 0x4c, 0x4d, 0x54, 0x55,
    0x71, 0x72
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 160, .range_length = 96, .glyph_id_start = 96,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 268, .range_length = 115, .glyph_id_start = 192,
        .unicode_list = unicode_list_2, .glyph_id_ofs_list = NULL, .list_length = 10, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 1, 0, 2, 0, 0, 0, 0,
    2, 3, 0, 0, 0, 4, 0, 4,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 6, 7, 8, 9, 10, 11,
    0, 12, 12, 13, 14, 15, 12, 12,
    9, 16, 17, 18, 0, 19, 13, 20,
    21, 22, 23, 24, 25, 0, 0, 0,
    0, 0, 26, 27, 28, 0, 29, 30,
    0, 31, 0, 0, 32, 0, 31, 31,
    33, 27, 0, 34, 0, 35, 0, 36,
    37, 38, 36, 39, 40, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 6, 6, 6, 6, 0, 8,
    10, 10, 10, 10, 12, 12, 12, 12,
    9, 12, 9, 9, 9, 9, 9, 0,
    0, 13, 13, 13, 13, 23, 0, 0,
    26, 26, 26, 26, 26, 26, 0, 28,
    29, 29, 29, 29, 0, 0, 0, 0,
    0, 31, 33, 33, 33, 33, 33, 0,
    0, 0, 0, 0, 0, 36, 27, 36,
    8, 28, 10, 29, 0, 34, 0, 0,
    24, 39
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 1, 0, 2, 0, 0, 0, 3,
    2, 0, 4, 5, 0, 6, 7, 6,
    8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    9, 0, 10, 0, 11, 0, 0, 0,
    11, 0, 0, 12, 0, 0, 0, 0,
    11, 0, 11, 0, 13, 14, 15, 16,
    17, 18, 19, 20, 0, 0, 21, 0,
    0, 0, 22, 0, 23, 23, 23, 24,
    23, 0, 0, 0, 0, 0, 25, 25,
    26, 25, 23, 27, 28, 29, 30, 31,
    32, 33, 31, 34, 0, 0, 35, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 36, 0, 7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 37, 0, 0, 0, 0,
    10, 10, 10, 10, 10, 10, 38, 11,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 11, 11, 11, 11, 11, 0,
    11, 15, 15, 15, 15, 19, 0, 0,
    22, 22, 22, 22, 22, 22, 39, 23,
    23, 23, 23, 23, 0, 0, 0, 0,
    0, 25, 26, 26, 26, 26, 26, 0,
    40, 30, 30, 30, 30, 31, 0, 31,
    11, 23, 0, 23, 0, 0, 13, 28,
    20, 34
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, -5, 0,
    -2, -5, 0, -6, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2,
    1, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -13, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -17,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, 0, 0,
    -5, 0, -1, 0, 0, -10, -1, -7,
    -5, 0, -7, 0, 0, 0, 0, 0,
    0, -1, 0, 0, -1, -1, -4, -3,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, -2,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    0, -2, 0, 0, 0, -2, 0, -2,
    0, -2, -3, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 0, 0,
    0, 0, 0, 0, 0, 0, -1, -1,
    0, -1, 0, 0, 0, -1, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -18, 0, 0,
    0, -13, 0, -21, 0, 2, 0, 0,
    0, 0, 0, 0, 0, -3, -2, 0,
    0, -2, -2, 0, 0, -2, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, -2, 0, 0,
    0, 1, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    -2, -2, 0, 0, 0, -2, -3, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -26, 0, 0, 0, 0, 0, 0,
    0, 1, -5, 0, 0, -21, -4, -14,
    -11, 0, -19, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, -10, -7,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -25, 0, 0,
    0, -11, 0, -16, 0, 0, 0, 0,
    0, -2, 0, -2, 0, -1, -1, 0,
    0, -1, 0, 0, 1, 0, 1, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, -2,
    -2, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, -1,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -3, 0, 0, 0, 0, -17, -18, 0,
    0, -6, -2, -19, -1, 1, 0, 1,
    1, 0, 1, 0, 0, -9, -8, 0,
    -9, -8, -6, -9, 0, -7, -6, -4,
    -6, -5, 0, -26, -17, -14, -9, -7,
    0, 0, 0, 2, 0, -18, -3, 0,
    0, -6, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, -4, -3, 0,
    0, -4, -2, 0, 0, -2, -1, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, -10, -5, 0,
    0, -3, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 1, -3, -2, 0,
    0, -2, -2, 0, 0, -1, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, -2, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, -2, 0, 0, 0, -2, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 2, -4, -16, -4, 0,
    0, -7, -2, -7, -1, 1, -7, 1,
    1, 1, 1, 0, 1, -6, -5, -2,
    -3, -5, -3, -5, -2, -3, -2, 0,
    -2, -2, 1, -6, -4, -7, -5, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, -2, 0, 0, 0, -1, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -1, -1, 0, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -11, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    -2, -1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, -10, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, -1, 1,
    0, -2, 0, 0, 4, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 1,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -10, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, -1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 40,
    .right_class_cnt     = 40,
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
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 1,
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
const lv_font_t ui_font_Roboto_Reg_10 = {
#else
lv_font_t ui_font_Roboto_Reg_10 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_ROBOTO_REG_10*/

