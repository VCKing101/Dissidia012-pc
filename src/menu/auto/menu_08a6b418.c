#include "menu/auto_region2.h"

extern unsigned char D_08A6C638[];

/* Panel6C638 constructor: builds its Panel58420. */
Panel6C638 *func_08A6B418(Panel6C638 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A6C638;
    func_08A573B4(&self->view);
    func_08A6B4C4(self);
    return self;
}
