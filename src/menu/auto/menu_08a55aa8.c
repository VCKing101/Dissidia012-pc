#include "menu/auto_region2.h"

/* Item58380: clears its two 0x80-entry tables. */
void func_08A55AA8(Item58380 *self)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        self->field_134[i] = 0;
        self->field_334[i] = 0;
    }
}
