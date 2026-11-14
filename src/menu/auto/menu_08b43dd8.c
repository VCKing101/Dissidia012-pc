#include "menu/auto_region4.h"

extern unsigned char D_08B44E90[];

/* Obj43DD8 constructor. */
Obj43DD8 *func_08B43DD8(Obj43DD8 *self)
{
    self->vtable = D_08B44E90;
    self->field_04 = 0;
    self->flags = 0;
    return self;
}
