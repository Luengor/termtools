#include <stdio.h>

#include "../src/screen.h"
#include "../src/term.h"

#define SCREEN_SIZE 16

int main(int argc, char *argv[])
{
    init_term();
    clear_screen();

    int width, height;
    get_term_size(&width, &height);
    height *= 2;

    screen_t screen = screen_init(width, height);

    double dt;
    double g;
    for (unsigned long frame = 0;;frame++)
    {
        g = (frame * dt * 20);
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
            {
                screen_set(x, y, rgb2color(
                        (int)((double)x / width * 256),
                        (int)g % 256, 
                        (int)((double)y / height * 256)
                        ), &screen);
            }

        dt = wait_for_frame(10);
        screen_print(&screen);

        if (kbhit() && getch() == 27)
            break;
    }

    return 0;
}

