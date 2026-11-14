#include "menu/auto_region2.h"

/* Panel5FA00: opens on value. */
void func_08A5F140(Panel5FA00 *self, int value)
{
    self->field_384 = value;
    func_08A5F178(self);
    func_08A5F8F8(self);
    func_08A5F830(self);
}
