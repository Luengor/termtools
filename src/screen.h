#ifndef _INCLUDE_SCREEN_H_
#define _INCLUDE_SCREEN_H_

#include <stdbool.h>
#include "common.h"

/// Constants

#define TABLE_LENGTH 5
#define CSI "\e["

#define WHITE rgb2color(255, 255, 255)
#define RED rgb2color(255, 0, 0)
#define GREEN rgb2color(0, 255, 0)
#define BLUE rgb2color(0, 0, 255)
#define BLACK rgb2color(0, 0, 0)


/// Macros

#define cursor_move(y, x) printf(CSI "%d;%dH", y, x)

#define foreground_setrgb(r, g, b) printf(CSI "38;2;%d;%d;%dm", r, g, b)
#define background_setrgb(r, g, b) printf(CSI "48;2;%d;%d;%dm", r, g, b)

#define foreground_setcolor(c) printf(CSI "38;2;%d;%d;%dm", \
        c.r, c.g, c.b);
#define background_setcolor(c) printf(CSI "48;2;%d;%d;%dm", \
        c.r, c.g, c.b);

#define rgb2color(r, g, b) (color_t){(r), (g), (b)}
#define colorp2pixel(fgc, bgc, p) (pixel_t){        \
    .pattern = (p),                                 \
    .fg = (fgc),                                    \
    .bg = (bgc),                                    \
}

#define rgb2pixel(r, g, b) (pixel_t){               \
    .pattern = 0b1111,                              \
    .fg = rgb2color(r, g, b),                       \
}

#define clear_screen() printf(CSI "2J")


/// Structs and td

typedef struct {
    byte r, g, b;
} color_t;

typedef struct {
    color_t fg, bg;
    byte pattern;
} pixel_t;

typedef struct {
    int width, height;
    bool changed;
    pixel_t *screen_old;
    pixel_t *screen;
} screen_t;


bool pixel_eq(pixel_t p1, pixel_t p2);
bool color_eq(color_t c1, color_t c2);

screen_t screen_init(int width, int height);
void screen_fill(pixel_t p, screen_t *screen);
void screen_print(screen_t *screen);
void screen_set(int x, int y, pixel_t p, screen_t *screen);

double wait_for_frame(double framerate);

#endif

