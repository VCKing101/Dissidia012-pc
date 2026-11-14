#include "main/auto_region2.h"

/* Sets the four pairs at field_160 to -1. */
void func_0882FE00(Obj82F470 *self)
{
    int i;

    i = 0;
    do {
        self->field_160[i].field_00 = 0xffff;
        self->field_160[i].field_02 = 0xffff;
        i = i + 1;
    } while (i < 4);
}
