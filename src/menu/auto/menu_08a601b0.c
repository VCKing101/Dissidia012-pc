#include "menu/auto_region2.h"

extern unsigned char D_08A60670[];

/* Panel60670 constructor. */
Panel60670 *func_08A601B0(Panel60670 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A60670;
    func_08A601EC(self);
    return self;
}
