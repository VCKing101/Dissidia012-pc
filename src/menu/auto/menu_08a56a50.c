#include "menu/auto_region2.h"

extern unsigned char D_08A583E0[];

/* Item583E0 constructor. */
Item583E0 *func_08A56A50(Item583E0 *self)
{
    func_08A54F38(&self->base);
    self->base.vtable = D_08A583E0;
    func_08A56AE8(self);
    return self;
}
