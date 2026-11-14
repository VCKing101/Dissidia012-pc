#include "menu/auto_region2.h"

extern unsigned char D_08A5FA00[];

/* Panel5FA00 constructor. */
Panel5FA00 *func_08A5F030(Panel5FA00 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A5FA00;
    func_08A5F06C(self);
    return self;
}
