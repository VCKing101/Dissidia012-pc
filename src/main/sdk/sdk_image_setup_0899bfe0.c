/* Image library: derive an image's flags, texture scale and buffer sizes
 * from its GIM image and palette headers. */
#include "main/sdk_gcc1_types.h"

void func_0899C2C4(SdkImage *img, int arg);

void func_0899BFE0(SdkImage *img)
{
    SdkGimImage *image;
    SdkGimImage *palette;
    int width, height;
    int pot_width, pot_height;
    int mask;
    int n;
    int i;

    image = img->image;
    img->flags = 0;
    if (image != 0) {
        if (image->level_count >= 2)
            img->flags = 1;
        if (image->frame_count >= 2)
            img->flags |= 2;
        width = image->width;
        height = image->height;
        pot_width = 1 << (32 - __builtin_allegrex_clz(width - 1));
        pot_height = 1 << (32 - __builtin_allegrex_clz(height - 1));
        if (width != pot_width || height != pot_height) {
            img->flags |= 0x60;
            img->f_20 = 0;
            img->f_24 = 0;
            img->scale_u = (float)width / (float)pot_width;
            img->scale_v = (float)height / (float)pot_height;
        }
        mask = image->pitch_align * 8 / image->bpp - 1;
        img->f_1C = (width + mask) & ~mask;
    }
    palette = img->palette;
    if (palette != 0) {
        if (palette->level_count >= 2)
            img->flags |= 4;
        if (palette->frame_count >= 2)
            img->flags |= 8;
        n = (palette->width >> 4) * palette->level_count;
        if (palette->format == 3)
            n *= 2;
        if (n > 32)
            n = 32;
        img->f_1E = n;
    }
    if (img->flags & 0xA) {
        img->flags |= 0x10;
    } else {
        for (i = 0; i < img->num_sequences; i++) {
            if (img->sequences[i]->f_0E != 0) {
                img->flags |= 0x10;
                break;
            }
        }
    }
    func_0899C2C4(img, img->f_60);
}
