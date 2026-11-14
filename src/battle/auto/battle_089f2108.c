#include "battle/auto_region1.h"

/* PartF2908 release: both members, the resource at field_04, then a reset. */
void func_089F2108(PartF2908 *self)
{
    func_089F7D0C(&self->field_28);
    func_089F7D0C(&self->field_08);
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089F20C4(self);
}
