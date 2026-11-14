/* Sets the third parameter blob; 1 when its magic is 0x20070422. */
#include "main/agent17_types.h"

int func_088AA47C(FxSlots *slots, FxBlob *blob)
{
    slots->third = blob;
    if (blob->magic != 0x20070422) {
        return 0;
    }
    slots->third_data = blob->data;
    slots->third_size = blob->size;
    return 1;
}
