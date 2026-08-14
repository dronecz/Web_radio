#pragma once

#ifndef DISPLAY_PIN_DC
#define DISPLAY_PIN_DC 17
#endif

#ifndef DISPLAY_PIN_CS
#define DISPLAY_PIN_CS 10
#endif

#ifndef DISPLAY_PIN_SCK
#define DISPLAY_PIN_SCK 12
#endif

#ifndef DISPLAY_PIN_MOSI
#define DISPLAY_PIN_MOSI 11
#endif

#ifndef DISPLAY_PIN_MISO
#define DISPLAY_PIN_MISO GFX_NOT_DEFINED
#endif

#ifndef DISPLAY_PIN_RST
#define DISPLAY_PIN_RST 18
#endif

#ifndef DISPLAY_BL_PIN
#define DISPLAY_BL_PIN 16
#endif

#ifndef DISPLAY_ROTATION
#if defined(small)
#define DISPLAY_ROTATION 3
#else
#define DISPLAY_ROTATION 1
#endif
#endif

#ifndef DISPLAY_ST7789_IS_IPS
#define DISPLAY_ST7789_IS_IPS 1
#endif

#ifndef DISPLAY_DRIVER_ST7789
#if defined(small)
#define DISPLAY_DRIVER_ST7789 1
#else
#define DISPLAY_DRIVER_ST7789 0
#endif
#endif

#ifndef DISPLAY_DRIVER_ST7796
#if defined(large)
#define DISPLAY_DRIVER_ST7796 1
#else
#define DISPLAY_DRIVER_ST7796 0
#endif
#endif

#ifndef DISPLAY_DRIVER_ILI9341
#define DISPLAY_DRIVER_ILI9341 0
#endif

#if (DISPLAY_DRIVER_ST7789 + DISPLAY_DRIVER_ST7796 + DISPLAY_DRIVER_ILI9341) != 1
#error "Exactly one display driver must be enabled via DISPLAY_DRIVER_ST7789/ST7796/ILI9341"
#endif

#ifndef DISPLAY_WIDTH
#if defined(small)
#define DISPLAY_WIDTH 320
#else
#define DISPLAY_WIDTH 480
#endif
#endif

#ifndef DISPLAY_HEIGHT
#if defined(small)
#define DISPLAY_HEIGHT 240
#else
#define DISPLAY_HEIGHT 320
#endif
#endif

#ifndef DISPLAY_FORCE_RESOLUTION
#define DISPLAY_FORCE_RESOLUTION 0
#endif

#ifndef DISPLAY_CANVAS_WIDTH
#if defined(small)
#define DISPLAY_CANVAS_WIDTH 285
#else
#define DISPLAY_CANVAS_WIDTH 385
#endif
#endif

#ifndef DISPLAY_CANVAS_HEIGHT
#if defined(small)
#define DISPLAY_CANVAS_HEIGHT 30
#else
#define DISPLAY_CANVAS_HEIGHT 40
#endif
#endif
