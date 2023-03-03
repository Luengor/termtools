#include "term.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_term()
{
    // Save original options 
    tcgetattr(STDIN_FILENO, &orig_term);

    // Restore terminal on exit
    atexit(restore_term);

    // Set non canonical
    tcgetattr(STDIN_FILENO, &curr_term);

    curr_term.c_lflag &= ~(ICANON);
    curr_term.c_lflag &= ~(ECHO);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &curr_term);

    // Hide cursor
    printf("\e[?25l");
}

void restore_term()
{
    // Restore
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    // Show cursor
    printf("\e[?25h");
}

void get_term_size(int *width, int *height)
{
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
}

double wait_for_frame(double framerate)
{
    static double prev = -1;
    double wait_time = 1.0 / framerate;

    // Init
    if (prev == -1) 
    {
        prev = (double)clock() / CLOCKS_PER_SEC;
        return 0;
    }

    // Normal it
    double dt = ((double)clock() / CLOCKS_PER_SEC) - prev;
    if (dt <= wait_time)
    {
        struct timespec tim, tim2;

        tim.tv_sec = (int)(wait_time- dt);
        tim.tv_nsec = (1000000000L * (wait_time- dt));

        nanosleep(&tim, &tim2);

        dt += (wait_time - dt);
        prev =  (double)clock() / CLOCKS_PER_SEC;
        return dt;
    }
    else
    {
        prev =  (double)clock() / CLOCKS_PER_SEC;
        return dt;
    }
}

