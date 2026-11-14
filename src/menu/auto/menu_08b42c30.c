#include "menu/auto_region4.h"

extern unsigned char D_08B43260[];

/* Obj42C30 constructor. */
Obj42C30 *func_08B42C30(Obj42C30 *self)
{
    self->vtable = D_08B43260;
    func_08B42C68(self);
    return self;
}
