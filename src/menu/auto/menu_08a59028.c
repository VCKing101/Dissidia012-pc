#include "menu/auto_region2.h"

extern unsigned char D_08A5B5C0[];

/* Part5B5C0 constructor. */
Part5B5C0 *func_08A59028(Part5B5C0 *self)
{
    func_08A58740(&self->base);
    self->base.vtable = D_08A5B5C0;
    func_08A59064(self);
    return self;
}
