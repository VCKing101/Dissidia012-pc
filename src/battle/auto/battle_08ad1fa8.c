#include "battle/auto_region4.h"

/* ObjD2988 constructor: stores the value. */
ObjD2988 *func_08AD1FA8(ObjD2988 *self, int value)
{
    self->vtable = D_08AD2988;
    self->field_04 = value;
    return self;
}
