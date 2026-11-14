#include "battle/auto_region2.h"

/* Part11A58 release: the member, the resource in field_04, then a reset. */
void func_08A11724(Part11A58 *self)
{
    func_089F7D0C(&self->field_08);
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A11704(self);
}
