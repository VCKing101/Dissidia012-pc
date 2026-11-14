#include "menu/auto_region2.h"

extern unsigned char D_08A67F88[];

/* Page67F88 constructor. */
Page67F88 *func_08A644A4(Page67F88 *self)
{
    func_08A638B0(&self->base);
    self->base.vtable = D_08A67F88;
    func_08A644E0(self);
    return self;
}
