#include "term.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

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

