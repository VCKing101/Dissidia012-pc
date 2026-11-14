#include "main/auto_region2.h"

/* Clears the four quads at field_28. */
void func_0887BBC8(Obj87BBC8 *self)
{
    int i;

    i = 0;
    do {
        self->field_28[i].field_00 = 0;
        self->field_28[i].field_02 = 0;
        self->field_28[i].field_04 = 0;
        self->field_28[i].field_06 = 0;
        i = i + 1;
    } while (i < 4);
}
