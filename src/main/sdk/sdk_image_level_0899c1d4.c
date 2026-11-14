/* Image library: pixel data of one level and frame of the image / palette. */
#include "main/sdk_gcc1_types.h"

void *func_0899C1D4(SdkImage *img, int level, int frame)
{
    SdkGimImage *image;
    void **offsets;
    int levels;
    int frames;

    if (img == 0)
        return 0;
    image = img->image;
    if (image == 0)
        return 0;
    levels = image->level_count;
    frames = image->frame_count;
    if (level < 0 || level >= levels)
        return 0;
    frame %= frames;
    if (frame < 0)
        frame += frames;
    offsets = (void **)((char *)image + image->index_start);
    return offsets[levels * frame + level];
}

void *func_0899C24C(SdkImage *img, int level, int frame)
{
    SdkGimImage *palette;
    void **offsets;
    int levels;
    int frames;

    if (img == 0)
        return 0;
    palette = img->palette;
    if (palette == 0)
        return 0;
    levels = palette->level_count;
    frames = palette->frame_count;
    if (level < 0 || level >= levels)
        return 0;
    frame %= frames;
    if (frame < 0)
        frame += frames;
    offsets = (void **)((char *)palette + palette->index_start);
    return offsets[levels * frame + level];
}

void func_0899C348(SdkImage *img, int shift, unsigned int mask, int arg);

void func_0899C2C4(SdkImage *img, int plane)
{
    SdkGimImage *image;
    unsigned int planes;
    unsigned int bit;
    int shift;

    if (img == 0)
        return;
    img->f_60 = plane;
    image = img->image;
    if (image == 0)
        return;
    planes = image->plane_mask;
    if (planes == 0) {
        func_0899C348(img, 0, 0xFF, 0);
        return;
    }
    planes ^= planes >> 1;
    bit = 1;
    while (--plane >= 0)
        bit += (planes - bit) ^ planes;
    bit = (planes - bit) ^ planes;
    shift = __builtin_allegrex_ctz(bit);
    func_0899C348(img, shift, bit >> shift, 0);
}
