#include "battle/auto_region2.h"

/* Unit036D8 constructor. */
Unit036D8 *func_08A021B8(Unit036D8 *self)
{
    func_089F12F0(&self->base);
    self->base.vtable = D_08A036D8;
    func_08A02258(self);
    return self;
}
