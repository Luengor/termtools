/// Includes

#include "screen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void draw_pixel(int x, int y, screen_t *screen)
{
    int y2 = y / 2 * 2;
    cursor_move(y / 2 + 1, x + 1);

    foreground_setcolor(screen->screen[y2 * screen->width + x]);
    background_setcolor(screen->screen[(y2 + 1) * screen->width + x]);

    printf("▀");
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
    for (int row = 0; row < screen->height; row++)
    {
        for (int column = 0; column < screen->width; column++)
        {
            color_t c1 = screen->screen[column + row * screen->width];
            color_t cold = screen->screen_old[column + row * screen->width];

            if (!color_eq(c1, cold))
                // This draws the same pixel twice!!
                // Maybe optimize that :)
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

void screen_set(int x, int y, color_t c, screen_t *screen)
{
    color_t *old = screen->screen + x + y * screen->width;

    if (color_eq(c, *old))
        return;

    *old = c;
    screen->changed = true;
}

