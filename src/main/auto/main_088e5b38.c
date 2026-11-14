#include "main/auto_region4.h"

/* Obj9CC610: clears its 32 words. */
void func_088E5B38(Obj9CC610 *self, int arg)
{
    int i;

    for (i = 0; i < 0x20; i++) {
        self->field_00[i] = 0;
    }
}
