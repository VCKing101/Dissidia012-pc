#include "menu/auto_region2.h"

extern unsigned char D_08A5EED8[];

/* Panel5EED8 constructor: builds its Panel60670. */
Panel5EED8 *func_08A5DE10(Panel5EED8 *self)
{
    func_08A51DD0(&self->base);
    self->base.vtable = D_08A5EED8;
    func_08A601B0(&self->field_A0);
    func_08A5DE54(self);
    return self;
}
