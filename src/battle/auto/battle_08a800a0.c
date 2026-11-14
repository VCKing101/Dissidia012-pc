#include "battle/auto_region3.h"

/* Part80208 constructor (vtable first, then the base). */
Part80208 *func_08A800A0(Part80208 *self)
{
    self->vtable = D_08A80208;
    func_08AAAE10(self->base);
    func_08A8014C(self);
    return self;
}
