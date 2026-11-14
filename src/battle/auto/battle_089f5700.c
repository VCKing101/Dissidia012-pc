#include "battle/auto_region1.h"

/* PartF5E58 constructor. */
PartF5E58 *func_089F5700(PartF5E58 *self)
{
    self->vtable = D_089F5E58;
    func_08993560(self->field_04, 3, sizeof(PartF5628), func_089F4FB8);
    func_089F57C8(self);
    return self;
}
