#ifndef _INCLUDE_SCREEN_H_
#define _INCLUDE_SCREEN_H_

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
#define foreground_setcolor(c) printf(CSI "38;2;%d;%d;%dm", \
        c.r, c.g, c.b);
#define background_setcolor(c) printf(CSI "48;2;%d;%d;%dm", \
        c.r, c.g, c.b);

#define rgb2color(r, g, b) (color_t){(r), (g), (b)}
#define gray2color(v) (color_t){v, v, v} 

#define cursor_move(y, x) printf(CSI "%d;%dH", y, x)
#define clear_screen() printf(CSI "2J")

/// Structs and td

typedef struct {
    byte r, g, b;
} color_t;

typedef struct {
    int width, height;
    bool changed;
    color_t *screen_old;
    color_t *screen;
} screen_t;


bool color_eq(color_t c1, color_t c2);

screen_t screen_init(int width, int height);
void screen_fill(color_t p, screen_t *screen);
void screen_print(screen_t *screen);
void screen_set(int x, int y, color_t p, screen_t *screen);

#endif

