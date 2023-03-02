#include <stdio.h>
#include <stdlib.h>
#include "../src/screen.h"
#include "../src/term.h"

int main(int argc, char *argv[])
{
    init_term();

    // Get terminal size
    int width, height;
    get_term_size(&width, &height);
    width /= 2;

    // Framerate
    float framerate = (argc == 2) ? atof(argv[1]) : 1; 

    // Screen
    screen_t screen = screen_init(width, height);
    clear_screen();

    screen_fill(rgb2pixel(0, 0, 0), &screen);

    // Main loop
    double dt = 0;
    double dttick;
    for (unsigned long tick = 0;;tick++, dttick += dt)
    {
        // Do the drawing
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                screen_set(x, y, colorp2pixel(
                        BLUE,
                        RED,
                        (1 << ((int)dttick) % 4)), &screen);

        // Flip screen
        dt = wait_for_frame(framerate);
        screen_print(&screen);

        cursor_move(1, 1);
        printf("%2.2lf %d", 1 / dt, (1 << ((int)dttick) % 4));
        fflush(stdout);
    }

    return 0;
}

