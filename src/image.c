#include "image.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <webp/decode.h>
#include <webp/types.h>

int image_read(const char *img_path, image_t *image)
{
    /// Read file
    // Open file
    FILE *fp = fopen(img_path, "rb");

    if (fp == NULL)
        return -1;

    // Get file size
    fseek(fp, 0L, SEEK_END);
    size_t sz = ftell(fp);
    rewind(fp);

    // Get data
    byte *data = malloc(sizeof(byte) * sz);
    fread(data, sz, 1, fp); 

    // Close file
    fclose(fp);

    /// Do wepm stuff
    WebPBitstreamFeatures features;
    VP8StatusCode status = WebPGetFeatures(data, sz, &features);

    if (status != VP8_STATUS_OK)
        return status; 

    image->width = features.width;
    image->height = features.height;

    // Get image
    byte *image_data = WebPDecodeRGB(data, sz, NULL, NULL); 
    image->image = malloc(image->width * image->height * sizeof(color_t));

    // memcpy seems risky
    for (int i = 0; i < image->height * image->width; i++)
    {
        image->image[i].r = image_data[i * 3 + 0];
        image->image[i].g = image_data[i * 3 + 1];
        image->image[i].b = image_data[i * 3 + 2];
    }

    // Cleanup
    WebPFree(image_data);
    free(data);

    return 0;
}

void image_rotate(int steps, image_t *img)
{
    #define w img->width
    #define h img->height
    #define i(_y,_x) ((_y) * w + (_x))
    #define invx w - 1 - x 
    #define invy h - 1 - y 

    // Allocate new image
    image_t *new_img = malloc(sizeof(image_t));
    new_img->width = w, new_img->height = h;
    new_img->image = malloc(w * h * sizeof(color_t));
    memcpy(new_img->image, img->image, w * h * sizeof(color_t));

    steps %= 4;

    // Move things
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            switch (steps)
            {
                // 90
                case -3:
                case 1:
                    img->image[i(y, x)] = new_img->image[i(invx, y)];
                    break;

                // 180
                case -2:
                case 2:
                    img->image[i(y, x)] = new_img->image[i(invy, invx)];
                    break;

                // 270
                case -1:
                case 3:
                    img->image[i(y, x)] = new_img->image[i(x, invy)];
                    break;

                // 360
                case 0:
                    break;
            }
        }
    }

    #undef w
    #undef h
    #undef i
    #undef invx 
    #undef invy 
}

