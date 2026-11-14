#include "menu/auto_region2.h"

extern unsigned char D_08A680D8[];

/* Page680D8 constructor. */
Page680D8 *func_08A65420(Page680D8 *self)
{
    func_08A638B0(&self->base);
    self->base.vtable = D_08A680D8;
    func_08A6545C(self);
    return self;
}
