/* Image library: put an image's GE commands into the current display list. */
#include "main/sdk_gcc1_types.h"

unsigned int *func_089A0144(unsigned int *size);
void func_089A0180(unsigned int *end);
int func_0899C764(SdkImage *img, unsigned int **list, unsigned int *size, unsigned int flags);

int func_0899D2F8(SdkImage *img)
{
    unsigned int size;
    unsigned int *list;
    int ret;

    list = func_089A0144(&size);
    ret = func_0899C764(img, &list, &size, 0xFFFF);
    func_089A0180(list);
    return ret;
}
