#include "menu/auto_region2.h"

/* Cell5C5C8: clears its three flags. */
void func_08A5C7A4(Cell5C5C8 *self)
{
    self->field_48 = 0;
    self->field_4A = 0;
    self->field_49 = 0;
    func_08A5CAE0(self);
}
