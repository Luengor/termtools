/// Includes

#include "screen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void draw_pixel(int x, int y, screen_t *screen)
{
    int y2 = y / 2 * 2;
    color_t fg = screen->screen[y2 * screen->width + x];
    color_t bg = screen->screen[(y2 + 1) * screen->width + x];

    printf(MOVE FOREGROUND BACKGROUND "▀",
            y / 2 + 1, x + 1,   // MOVE y x
            fg.r, fg.g, fg.b,   // FOREGROUNG r g b
            bg.r, bg.g, bg.b);  // BACKGROUND r g b
}


bool color_eq(color_t c1, color_t c2)
{
    return (c1.r == c2.r && c1.b == c2.b && c1.g == c2.g);
}

screen_t screen_init(int width, int height)
{
    screen_t screen = {
        .width = width, 
        .height = height,
        .changed = true,
        .screen = malloc(width * height * sizeof(color_t)),
        .screen_old = malloc(width * height * sizeof(color_t))};

    return screen;
}

void screen_print(screen_t *screen)
{
    // Move cursor up
    cursor_move(1, 1);

    // Check if changed
    if (!screen->changed)
        return;

    // Print diff 
    for (int row = 0; row < screen->height; row+=2)
    {
        for (int column = 0; column < screen->width; column++)
        {
            if (pixel_changed(column, row, screen) || 
                    pixel_changed(column, row + 1, screen))
                draw_pixel(column, row, screen);

        }
    }

    fflush(stdout);

    // Copy screen to old
    memcpy(screen->screen_old, screen->screen, screen->width * screen->height);
    screen->changed = false;
}

void screen_fill(color_t c, screen_t *screen)
{
    for (int i = 0; i < screen->width * screen->height; i++)
        screen->screen[i] = c;

    screen->changed = true;
}

void screen_plot(int x, int y, color_t c, screen_t *screen)
{
    color_t *old = screen->screen + x + y * screen->width;

    if (color_eq(c, *old))
        return;

    *old = c;
    screen->changed = true;
}

void screen_plot_box(int x1, int y1, int x2, int y2,
        color_t color, screen_t *screen)
{
    for (; y1 <= y2; y1++)
        for (int x = x1; x <= x2; x++)
            screen->screen[y1 * screen->width + x] = color;
}

void screen_plot_image(int x, int y, image_t *image, screen_t *screen)
{
    #define sw screen->width
    #define sh screen->height
    #define iw image->width
    #define ih image->height


    for (int iy = y; (iy - y) < ih && iy < sh; iy++)
        for (int ix = x; (ix - x) < iw && ix < sw; ix++)
            screen->screen[iy * sw + ix] = image->image[(iy - y) * iw + (ix - x)];


    #undef sw
    #undef sh
    #undef iw
    #undef ih
}

