/* Image library: drop an image's chunk copies and sequence list. */
#include "main/sdk_gcc1_types.h"

void func_0899C6DC(SdkImage *img);

void func_0899BEE0(SdkImage *img)
{
    int i;
    SdkGimSequence *seq;

    if (img == 0)
        return;
    func_0899C6DC(img);
    if (img->image != 0) {
        if (--img->image->refs == 0)
            func_0899B830(img->image);
        img->image = 0;
    }
    if (img->palette != 0) {
        if (--img->palette->refs == 0)
            func_0899B830(img->palette);
        img->palette = 0;
    }
    for (i = 0; i < img->num_sequences; i++) {
        seq = img->sequences[i];
        if (--seq->refs == 0)
            func_0899B804(seq);
    }
    func_0899B804(img->sequences);
    img->sequences = 0;
    img->num_sequences = 0;
}
