#include "battle/auto_region5.h"

/* Obj3F118 constructor: builds the Obj3E2E8 base, then clears its two
 * words. */
Obj3F118 *func_08B3E3C8(Obj3F118 *self)
{
    func_08B3D6C0(&self->base);
    self->base.vtable = D_08B3F118;
    self->field_04 = 0;
    self->field_08 = 0;
    return self;
}
