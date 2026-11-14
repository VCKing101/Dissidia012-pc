#include "menu/auto_region2.h"

extern unsigned char D_08A694A8[];

/* Panel694A8 constructor. */
Panel694A8 *func_08A68388(Panel694A8 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A694A8;
    func_08A683C4(self);
    return self;
}
