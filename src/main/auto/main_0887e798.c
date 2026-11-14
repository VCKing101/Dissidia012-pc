#include "main/auto_region2.h"

/* Saves the pair at field_10 and replaces it with (0, value). */
void func_0887E798(Obj87E798 *self, int value)
{
    self->field_20 = self->field_10;
    self->field_24 = self->field_14;
    self->field_10 = 0;
    self->field_14 = value;
}
