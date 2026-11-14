#include "battle/auto_region1.h"

/* PartEA518 constructor. */
PartEA518 *func_089E9E10(PartEA518 *self)
{
    self->vtable = D_089EA518;
    func_0887A61C(self->block);
    func_08993560(self->entries, 2, 0x84C, func_089EA530);
    func_089E9EF4(self);
    return self;
}
