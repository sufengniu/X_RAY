#ifndef TIF_H_
#define TIF_H_

#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>

#define BIT_PER_SAMPLE          16      // defined by camera property

typedef struct tiff_info{
        unsigned short type;    /* little or big endian */
        int width;
        int length;
        int depth;
        unsigned short photo_metric;
        uint32 spp;     // sample per pixel, 16 bits per pixel for ANL camera
        uint16 bps;     // bit per sample, default is 1
        int line_size;
        int image_size;
        uint16 config;
} tiff_info;

int tif_load(int argc, char **argv, uint16 *input_image);
void tif_release(uint16 *image);

#endif /* tif.h */
