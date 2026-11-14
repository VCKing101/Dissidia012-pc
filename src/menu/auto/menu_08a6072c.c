#include "menu/auto_region2.h"

extern unsigned char D_08A61C40[];

/* Panel61C40 constructor. */
Panel61C40 *func_08A6072C(Panel61C40 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A61C40;
    func_08A60768(self);
    return self;
}
