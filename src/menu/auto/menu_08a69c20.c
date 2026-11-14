#include "menu/auto_region2.h"

extern unsigned char D_08A6B170[];

/* Panel6B170 constructor. */
Panel6B170 *func_08A69C20(Panel6B170 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A6B170;
    func_08A69CB8(self);
    return self;
}
