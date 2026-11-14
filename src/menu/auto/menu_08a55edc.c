#include "menu/auto_region2.h"

extern unsigned char D_08A583A0[];

/* Item583A0 constructor. */
Item583A0 *func_08A55EDC(Item583A0 *self)
{
    func_08A54F38(&self->base);
    self->base.vtable = D_08A583A0;
    func_08A55F74(self);
    return self;
}
