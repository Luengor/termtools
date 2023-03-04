#include "term.h"
#include "screen.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_term()
{
    // Save original options 
    tcgetattr(STDIN_FILENO, &orig_term);
    tcgetattr(STDIN_FILENO, &curr_term);

    // Restore terminal on exit
    atexit(restore_term);

    // Disable canonical mode 
    curr_term.c_lflag &= ~(ICANON);
    // Disable echo
    curr_term.c_lflag &= ~(ECHO);
    // Wait until at least 1 keystroke is availalble 
    curr_term.c_cc[VMIN] = 1;
    // No timeout
    curr_term.c_cc[VTIME] = 0;
    
    // Update terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &curr_term);

    // Hide cursor
    printf("\e[?25l");

    // Clean screen
    clear_screen();
}

void restore_term()
{
    // Restore setting
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    // Set default colors
    printf(CSI "39m");
    printf(CSI "49m");

    // Show cursor and move it up
    printf("\e[?25h");
    cursor_move(1, 1);

    // Clear screen
    clear_screen();
}

void get_term_size(int *width, int *height)
{
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
}


bool kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

int getch()
{
    int r;
    byte c;

    if ((r = read(STDIN_FILENO, &c, sizeof(c))) < 0)
        return r;
    else
        return c;
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

