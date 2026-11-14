/* Image library: create an image object, optionally loading GIM data. */
#include "main/sdk_gcc1_types.h"

extern const float D_089D1300; /* 1.0f */
extern const float D_089D1304; /* -1000000.0f */
extern const float D_089D1308; /* 1000000.0f */
extern const float D_089D130C; /* 12.0f */

SdkImage *func_0899B9F0(void *data)
{
    SdkImage *img;

    img = func_0899B75C(sizeof(SdkImage));
    if (img == 0)
        return 0;
    img->refs = 1;
    img->flags = 0;
    img->f_08 = 0;
    img->image = 0;
    img->palette = 0;
    img->f_14 = 0;
    img->f_18 = 0;
    img->f_1C = 0;
    img->f_1E = 0;
    img->f_20 = 0;
    img->f_24 = 0;
    img->scale_u = D_089D1300;
    img->scale_v = D_089D1300;
    img->f_30 = D_089D1304;
    img->f_34 = D_089D1308;
    img->f_38 = D_089D130C;
    img->f_3C = 0;
    img->f_40 = 0;
    img->f_44 = 0;
    img->f_48 = 0;
    img->f_4C = 0;
    img->f_50 = 0;
    img->sequences = 0;
    img->num_sequences = 0;
    img->f_5C = -1;
    img->f_60 = 0;
    img->f_64 = 0;
    img->f_68 = 0xFF;
    img->f_6C = 0;
    img->f_70 = 1;
    img->f_74 = 0;
    img->f_78 = 1;
    img->f_7C = 1;
    img->f_80 = 7;
    img->f_84 = 0;
    img->f_88 = 0;
    if (data != 0 && !func_0899BC64(img, data)) {
        func_0899BB1C(img);
        return 0;
    }
    return img;
}

void func_0899BB1C(SdkImage *img)
{
    if (img == 0)
        return;
    if (--img->refs == 0) {
        func_0899BEE0(img);
        func_0899B804(img);
    }
}
