#include "main/auto_region1.h"

/* Reset of Obj9C74E8. */
void func_08819C1C(Obj9C74E8 *self)
{
    int i;

    self->active = 0;
    self->field_01 = 0;
    for (i = 0; i < 3; i++) {
        self->field_04[i] = 0;
    }
}
