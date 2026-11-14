/* Image library: load an image from memory, by file type. */
#include "main/sdk_gcc1_types.h"

int func_0899E998(void *data, unsigned int size);
SdkGimChunk *func_0899E9EC(void *data, unsigned int size, int arg);
int func_0899E4B0(SdkImage *img, void *data, unsigned int size, int arg);
int func_0899D348(SdkImage *img, void *data, unsigned int size, int arg);
int func_0899DEF4(SdkImage *img, void *data, unsigned int size, int arg);

int func_0899BB68(SdkImage *img, void *data, unsigned int size, int arg)
{
    unsigned int magic;
    SdkGimChunk *file;

    if (img == 0)
        return 0;
    if (data == 0)
        return 0;
    if (size != 0 && size < 4)
        return 0;
    magic = *(unsigned int *)data;
    if (magic == SDK_GIM_MAGIC) {
        if (!func_0899E998(data, size))
            return 0;
        file = func_0899E9EC(data, size, arg);
        if (file == 0)
            return 0;
        return func_0899BC64(img, file);
    }
    if (magic == SDK_TIM2_MAGIC)
        return func_0899E4B0(img, data, size, arg);
    if ((magic & 0xFFFF) == SDK_BMP_MAGIC)
        return func_0899D348(img, data, size, arg);
    return func_0899DEF4(img, data, size, arg);
}
