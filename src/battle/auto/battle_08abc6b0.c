#include "battle/auto_region4.h"

/* PartBC608 reset: clears field_28 and field_2C, then calls func_08ABC88C. */
void func_08ABC6B0(PartBC608 *self)
{
    self->field_28 = 0;
    self->field_2C = 0;
    func_08ABC88C(self);
}
