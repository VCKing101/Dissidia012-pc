#include "battle/auto_region2.h"

/* Part08880 release: the member, the resource in field_04, then a reset. */
void func_08A08480(Part08880 *self)
{
    func_089F7D0C(&self->field_08);
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A0845C(self);
}
