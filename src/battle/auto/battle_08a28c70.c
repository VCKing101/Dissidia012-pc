#include "battle/auto_region3.h"

/* Unit29370 constructor. */
Unit29370 *func_08A28C70(Unit29370 *self)
{
    func_089F12F0(&self->base);
    self->base.vtable = D_08A29370;
    func_08993560(self->field_10, 2, sizeof(Part268C8), func_08A261C8);
    func_08A255B0(&self->field_248);
    func_08A28D5C(self);
    return self;
}
