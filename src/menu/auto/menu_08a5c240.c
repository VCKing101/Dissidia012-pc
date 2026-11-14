#include "menu/auto_region2.h"

/* Action5C240: sets field_08 and field_04. */
void func_08A5C240(Action5C240 *self, unsigned int value, unsigned char flag)
{
    self->field_08 = value;
    self->field_04 = flag;
}
