#include "menu/auto_region2.h"

extern unsigned char D_08A69A78[];

/* Panel69A78 constructor. */
Panel69A78 *func_08A69600(Panel69A78 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A69A78;
    func_08A6963C(self);
    return self;
}
