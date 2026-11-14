#include "menu/auto_region2.h"

extern unsigned char D_08A5B578[];

/* Part5B578 constructor. */
Part5B578 *func_08A58868(Part5B578 *self)
{
    func_08A58740(&self->base);
    self->base.vtable = D_08A5B578;
    func_08A588A4(self);
    return self;
}
