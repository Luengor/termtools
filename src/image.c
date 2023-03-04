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

