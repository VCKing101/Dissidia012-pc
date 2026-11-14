#include "battle/auto_region3.h"

/* Part7ABD0 release: releases the members, then resets. */
void func_08A65F70(Part7ABD0 *self)
{
    func_08A94300(self->field_24);
    func_08A7E7E0(&self->field_14);
    func_08A7E7E0(&self->field_04);
    func_08A65F24(self);
}
