#include <stdio.h>
#include <math.h>

#include "../src/screen.h"
#include "../src/term.h"

#define SCREEN_SIZE 16

int main(int argc, char *argv[])
{
    init_term();
    clear_screen();

    int width, height;
    get_term_size(&width, &height);
    width /= 2;

    screen_t screen = screen_init(width, height);

    double dt;
    double g;
    for (unsigned long frame = 0;;frame++)
    {
        g = (frame * dt * 20);
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
            {
                screen_set(x, y, rgb2pixel(
                        (int)((double)x / width * 256),
                        (int)g % 256, 
                        (int)((double)y / height * 256)
                        ), &screen);
            }

        dt = wait_for_frame(30);
        screen_print(&screen);

        cursor_move(1, 1);
        printf("%.2lf", 1 / dt);
        fflush(stdout);
    }

    clear_screen();

    return 0;
}

