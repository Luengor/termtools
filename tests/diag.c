#include <stdio.h>
#include <stdlib.h>
#include "../src/screen.h"
#include "../src/term.h"

#define grad(v) ((int)(v) % 512 <= 255) ? ((int)v % 256) : (511 - (int)v % 512)

int main(int argc, char *argv[])
{
    init_term();

    // Get terminal size
    int width, height;
    get_term_size(&width, &height);
    height *= 2;

    // Framerate
    float framerate = (argc == 2) ? atof(argv[1]) : 1; 

    // Screen
    screen_t screen = screen_init(width, height);
    clear_screen();

    screen_fill(BLACK, &screen);

    // Main loop
    double dt = 0;
    double dttick;
    for (unsigned long tick = 0;;tick++, dttick += dt)
    {
        // Do the drawing
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                screen_set(x, y, gray2color(grad((x + y + dttick * 30))), &screen);

        // Flip screen
        dt = wait_for_frame(framerate);
        screen_print(&screen);
    }

    return 0;
}

