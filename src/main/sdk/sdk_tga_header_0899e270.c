/* Image library, TGA reader: the file header. */
#include "main/sdk_gcc1_types.h"

const unsigned char *func_0899E324(const unsigned char *p, unsigned char *out);
const unsigned char *func_0899E334(const unsigned char *p, unsigned short *out);

const unsigned char *func_0899E270(const unsigned char *p, SdkTgaHeader *h)
{
    p = func_0899E324(p, &h->id_length);
    p = func_0899E324(p, &h->colormap_type);
    p = func_0899E324(p, &h->image_type);
    p = func_0899E334(p, &h->colormap_first);
    p = func_0899E334(p, &h->colormap_length);
    p = func_0899E324(p, &h->colormap_bpp);
    p = func_0899E334(p, &h->x_origin);
    p = func_0899E334(p, &h->y_origin);
    p = func_0899E334(p, &h->width);
    p = func_0899E334(p, &h->height);
    p = func_0899E324(p, &h->bpp);
    p = func_0899E324(p, &h->descriptor);
    return p + h->id_length;
}
