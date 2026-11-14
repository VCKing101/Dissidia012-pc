/* Image library: is this a GIM file ("MIG.00.1PSP")? */
#include "main/sdk_gcc1_types.h"

int func_0899E998(void *data, unsigned int size)
{
    unsigned int *magic = data;

    if (data == 0)
        return 0;
    if (size != 0 && size < 32)
        return 0;
    if (magic[0] != SDK_GIM_MAGIC)
        return 0;
    if (magic[1] != 0x312E3030)
        return 0;
    return magic[2] == 0x00505350;
}
