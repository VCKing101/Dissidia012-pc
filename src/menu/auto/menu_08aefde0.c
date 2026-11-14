#include "menu/auto_region3.h"

extern unsigned char D_08AF00A0[];

/* ObjEFDE0 constructor. */
ObjEFDE0 *func_08AEFDE0(ObjEFDE0 *self, int arg1, int arg2)
{
    self->vtable = D_08AF00A0;
    self->field_04 = arg1;
    self->field_08 = arg2;
    return self;
}
