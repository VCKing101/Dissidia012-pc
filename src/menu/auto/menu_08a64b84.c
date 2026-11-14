#include "menu/auto_region2.h"

extern unsigned char D_08A68030[];

/* Page68030 constructor. */
Page68030 *func_08A64B84(Page68030 *self)
{
    func_08A638B0(&self->base);
    self->base.vtable = D_08A68030;
    func_08A64BC0(self);
    return self;
}
