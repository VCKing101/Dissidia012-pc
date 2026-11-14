#include "menu/auto_region2.h"

extern unsigned char D_08A58400[];

/* Item58400 constructor. */
Item58400 *func_08A56CD0(Item58400 *self)
{
    func_08A54F38(&self->base);
    self->base.vtable = D_08A58400;
    func_08A56D68(self);
    return self;
}
