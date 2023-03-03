#ifndef _INCLUDE_TERM_H_
#define _INCLUDE_TERM_H_

#include <termios.h>
#include "common.h"

#define INPUT_BUFFER_SIZE 10

static struct termios orig_term, curr_term;
static char input_buffer[INPUT_BUFFER_SIZE];
static unsigned int input_read;

void init_term();
void restore_term();
void get_term_size(int *width, int *height);

bool kbhit();
int getch();

double wait_for_frame(double framerate);

#endif

