#include "menu/auto_region2.h"

extern unsigned char D_08A5B530[];

/* Part base constructor. */
Part *func_08A58740(Part *self)
{
    self->vtable = D_08A5B530;
    func_08A58778(self);
    return self;
}
