#include "battle/auto_region4.h"

/* PartF2CBC setup: stores the values and the name. */
void func_08AF2CBC(PartF2CBC *self, int arg1, int arg2, const char *name,
                   int arg4, int arg5, int arg6)
{
    self->field_34 = arg1;
    self->field_38 = arg2;
    self->field_64 = arg4;
    self->field_68 = arg5;
    self->field_6C = arg6;
    func_089359A4(self->field_3C, name);
    self->field_72 = 0;
    self->field_74 = -1;
}
