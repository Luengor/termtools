#include <stdio.h>
#include <stdlib.h>

#include "../src/screen.h"
#include "../src/image.h"
#include "../src/term.h"

int main(int argc, char *argv[])
{
    // Read image
    if (argc != 2)
        return 1;

    image_t img;
    int r;
    if ((r = image_read(argv[1], &img)) != 0)
        return r;

    // Setup screen
    int swidth, sheight;
    get_term_size(&swidth, &sheight);
    sheight *= 2;

    if (swidth < img.width || sheight < img.height)
    {
        foreground_setcolor(RED);
        printf("2 smol\n");
        return 2;
    }
    init_term();

    screen_t screen = screen_init(swidth, sheight);

    screen_fill(WHITE, &screen);
    screen_plot_image(0, 0, &img, &screen);
    screen_print(&screen);

    while (true)
    {
        wait_for_frame(3);
        if (kbhit() && getch() == 27)
            break;
    }

    return 0;
}

