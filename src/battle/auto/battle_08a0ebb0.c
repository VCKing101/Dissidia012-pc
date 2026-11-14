#include "battle/auto_region2.h"

/* Part0F8D8 constructor. */
Part0F8D8 *func_08A0EBB0(Part0F8D8 *self)
{
    self->vtable = D_08A0F8D8;
    func_08993560(self->field_34, 2, sizeof(PartF8670), func_089F8208);
    func_08993560(self->field_A4, 2, sizeof(Part08388), func_08A08178);
    func_08A0ECB0(self);
    return self;
}
