#include "menu/auto_region2.h"

/* Request6E168 reset. */
void func_08A6E388(Request6E168 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = -1;
    self->kind = -1;
}
