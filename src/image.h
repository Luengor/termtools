#ifndef _INCLUDE_IMAGE_H_
#define _INCLUDE_IMAGE_H_

#include "common.h"
#include <stdio.h>

#define WHITE rgb2color(255, 255, 255)
#define RED rgb2color(255, 0, 0)
#define GREEN rgb2color(0, 255, 0)
#define BLUE rgb2color(0, 0, 255)
#define BLACK rgb2color(0, 0, 0)
#define NULL_COLOR (color_t){.null=true}

#define rgb2color(r, g, b) (color_t){(r), (g), (b)}
#define hex2color(h) (color_t){((h) >> 16) & 0xff, ((h) >> 8) & 0xff , (h) & 0xff}
#define gray2color(v) (color_t){(v), (v), (v)} 

typedef struct {
    byte r, g, b;
    bool null;
} color_t;

typedef struct {
    int width, height;
    color_t *image;
} image_t;


int image_read(const char *img_path, image_t *);
image_t image_from_mask(const char *mask, int width, color_t color); 
void image_rotate(int steps, image_t *);


#endif

