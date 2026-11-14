#include "menu/auto_region2.h"

extern unsigned char D_08A583C0[];

/* Item583C0 constructor. */
Item583C0 *func_08A5657C(Item583C0 *self)
{
    func_08A54F38(&self->base);
    self->base.vtable = D_08A583C0;
    func_08A56614(self);
    return self;
}
