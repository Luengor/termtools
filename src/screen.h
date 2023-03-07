#ifndef _INCLUDE_SCREEN_H_
#define _INCLUDE_SCREEN_H_

#include "common.h"
#include "image.h"

/// Constants

#define CSI "\e["
#define CLEAR CSI "2J"
#define MOVE CSI "%d;%dH"
#define FOREGROUND CSI "38;2;%d;%d;%dm"
#define BACKGROUND CSI "48;2;%d;%d;%dm"


/// Macros
#define foreground_setcolor(c) printf(FOREGROUND,           \
        c.r, c.g, c.b);
#define background_setcolor(c) printf(BACKGROUND,           \
        c.r, c.g, c.b);

#define pixel_changed(x, y, screen)                                 \
        !color_eq((screen)->screen[(y) * (screen)->width + (x)],    \
                 (screen)->screen_old[(y) * (screen)->width + (x)]) 

#define cursor_move(y, x) printf(MOVE, y, x)
#define clear_screen() printf(CLEAR)

/// Structs and td

typedef struct {
    int width, height;
    bool changed;
    color_t *screen_old;
    color_t *screen;
} screen_t;


bool color_eq(color_t c1, color_t c2);

screen_t screen_init(int width, int height);
void screen_print(screen_t *screen, bool redraw);

void screen_fill(color_t c, screen_t *screen);
void screen_plot(int x, int y, color_t c, screen_t *screen);
void screen_plot_box(int x1, int y1, int x2, int y2, color_t, screen_t *);
void screen_plot_image(int x, int y, image_t *img, screen_t *screen);

#endif

