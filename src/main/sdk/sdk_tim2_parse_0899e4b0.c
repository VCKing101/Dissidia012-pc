/* Image library: tim2_parse - read picture `index` of a TIM2 file into the image (pixels and CLUT). */
#include "main/agent34_sdk.h"

int func_0899E4B0(D012ImageObject *img, const u8 *data, u32 size, int index)
{
    const D012Tim2Header *file = (const D012Tim2Header *)data;
    const D012Tim2Picture *pic;
    int type;
    int i;

    if (img == 0)
        return 0;
    func_0899BEE0(img);
    if (index < 0 || index >= file->picture_count)
        return 0;
    if (file->align_128 == 0)
        pic = (const D012Tim2Picture *)(data + 0x10);
    else
        pic = (const D012Tim2Picture *)(data + 0x80);
    for (i = 0; i < index; i++)
        pic = (const D012Tim2Picture *)((const u8 *)pic + pic->total_size);

    if (pic->mipmap_count != 0) {
        int width = pic->width;
        int height = pic->height;
        const D012Tim2Mipmap *mip;
        const u8 *pixels;
        int level;
        int format = D_089D14C8[pic->image_type];

        if (!func_0899EAEC(img, format, width, height, pic->mipmap_count, 1, 0))
            return 0;
        mip = 0;
        if (pic->mipmap_count > 1)
            mip = (const D012Tim2Mipmap *)(pic + 1);
        pixels = (const u8 *)pic + pic->header_size;
        for (level = 0; level < pic->mipmap_count; level++) {
            func_0899E6F0(func_0899C1D4(img, level, 0), pixels, width, height, pic->image_type);
            if (mip != 0) {
                width = (width + 1) / 2;
                height = (height + 1) / 2;
                pixels += mip->sizes[level];
            }
        }
    }

    type = pic->clut_type & 7;
    if (type != 0) {
        int format = D_089D14C8[type];
        int colors = pic->clut_colors;
        const u8 *clut = (const u8 *)pic + pic->header_size + pic->image_size;
        void *pal;

        func_0899EE30(img, format, colors, 1, 1, 0);
        pal = func_0899C24C(img, 0, 0);
        if (pal == 0)
            return 0;
        func_0899E6F0(pal, clut, colors, 1, type);
        if (pic->clut_type >= 0)
            func_0899E900(pal, format, colors);
    }
    func_089C6040();
    return 1;
}
