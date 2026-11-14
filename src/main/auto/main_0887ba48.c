#include "main/auto_region2.h"

/* Sets field_00 and flag 0x2000. */
void func_0887BA48(Obj87B820 *self, int value)
{
    self->field_00 = value;
    self->flags = self->flags | 0x2000;
}
