#include "battle/auto_region2.h"

/* Part08158 constructor. */
Part08158 *func_08A06B70(Part08158 *self)
{
    self->vtable = D_08A08158;
    func_08A15968(self->field_20);
    func_08A15540(self->field_28);
    func_08A112A8(&self->field_34);
    func_089F0EB0(&self->field_48);
    func_089F5700(&self->field_5C);
    func_08993560(self->field_164, 2, sizeof(Part06B08), func_08A06198);
    func_08993560(self->field_1CC, 2, sizeof(Part10F98), func_08A10AC8);
    func_08993560(self->field_224, 2, sizeof(Part048C8), func_08A03880);
    func_08A06D0C(self);
    return self;
}
