/* Sets the second parameter blob; 1 when its magic is 0x20100205. */
#include "main/agent17_types.h"

int func_088AA440(FxSlots *slots, FxBlob *blob)
{
    slots->second = blob;
    if (blob->magic != 0x20100205) {
        return 0;
    }
    slots->second_data = blob->data;
    slots->second_size = blob->size;
    return 1;
}
