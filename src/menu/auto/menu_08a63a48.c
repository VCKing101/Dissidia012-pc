#include "menu/auto_region2.h"

extern unsigned char D_08A67EE0[];

/* Page67EE0 constructor. */
Page67EE0 *func_08A63A48(Page67EE0 *self)
{
    func_08A638B0(&self->base);
    self->base.vtable = D_08A67EE0;
    func_08A63A84(self);
    return self;
}
