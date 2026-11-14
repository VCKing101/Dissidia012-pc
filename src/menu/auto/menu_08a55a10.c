#include "menu/auto_region2.h"

extern unsigned char D_08A58380[];

/* Item58380 constructor. */
Item58380 *func_08A55A10(Item58380 *self)
{
    func_08A54F38(&self->base);
    self->base.vtable = D_08A58380;
    func_08A55AA8(self);
    return self;
}
