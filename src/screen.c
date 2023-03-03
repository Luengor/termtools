/// Includes

#include "screen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/// Globals :(

static char *block_table[] = {
    "  ",   // 0 0 0 0
    "▀ ",   // 1 0 0 0
    " ▀",   // 0 1 0 0 
    "▀▀",   // 1 1 0 0
            
    "▄ ",   // 0 0 1 0
    "█ ",   // 1 0 1 0
    "▄▀",   // 0 1 1 0
    "█▀",   // 1 1 1 0
    
    " ▄",   // 0 0 0 1
    "▀▄",   // 1 0 0 1
    " █",   // 0 1 0 1 
    "▀█",   // 1 1 0 1
            
    "▄▄",   // 0 0 1 1
    "█▄",   // 1 0 1 1
    "▄█",   // 0 1 1 1
    "██",   // 1 1 1 1
};


/// Macros

#define draw_pixel(p) do {                  \
    foreground_setcolor((p).fg);              \
    background_setcolor((p).bg);              \
    printf("%s", block_table[(p).pattern]);   \
    }while(0)



bool color_eq(color_t c1, color_t c2)
{
    return (c1.r == c2.r && c1.b == c2.b && c1.g == c2.g);
}

bool pixel_eq(pixel_t p1, pixel_t p2)
{
    return (color_eq(p1.fg, p2.fg) && color_eq(p1.bg, p2.bg) 
            && p1.pattern == p2.pattern);
}


screen_t screen_init(int width, int height)
{
    screen_t screen = {
        .width = width, 
        .height = height,
        .changed = true,
        .screen = malloc(width * height * sizeof(pixel_t)),
        .screen_old = malloc(width * height * sizeof(pixel_t))};

    screen_fill((pixel_t){.pattern = 10}, &screen);
    pixel_t *tmp = screen.screen;
    screen.screen = screen.screen_old;
    screen.screen_old = tmp;

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
            pixel_t p1 = screen->screen[column + row * screen->width];
            pixel_t pold = screen->screen_old[column + row * screen->width];

            if (!pixel_eq(p1, pold))
            {
                cursor_move(row + 1, column*2 + 1);
                draw_pixel(p1);
            }
        }
    }

    fflush(stdout);

    // Copy screen to old
    memcpy(screen->screen_old, screen->screen, screen->width * screen->height);
    screen->changed = false;
}

void screen_fill(pixel_t p, screen_t *screen)
{
    for (int i = 0; i < screen->width * screen->height; i++)
    {
        screen->screen[i] = p;
    }

    screen->changed = true;
}

void screen_set(int x, int y, pixel_t p, screen_t *screen)
{
    pixel_t *old = screen->screen + x + y * screen->width;

    if (pixel_eq(p, *old))
        return;

    *old = p;
    screen->changed = true;
}

