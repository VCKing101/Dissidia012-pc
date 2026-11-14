#include "menu/auto_region2.h"

extern unsigned char D_08A58360[];

/* Item base constructor. */
Item *func_08A54F38(Item *self)
{
    self->vtable = D_08A58360;
    func_08A54FA4(self);
    return self;
}
