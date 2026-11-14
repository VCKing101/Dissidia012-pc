#include "menu/auto_region2.h"

/* Panel69A78: sets fields 0x38..0x40. */
void func_08A69704(Panel69A78 *self, int arg1, int arg2, int arg3)
{
    self->field_38 = arg1;
    self->field_3C = arg2;
    self->field_40 = arg3;
}
