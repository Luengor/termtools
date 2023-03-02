#ifndef _INCLUDE_TERM_H_
#define _INCLUDE_TERM_H_

#include <termios.h>

static struct termios orig_term, curr_term;

void init_term();
void restore_term();
void get_term_size(int *width, int *height);


#endif

