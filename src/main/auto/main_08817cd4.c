#include "main/auto_region1.h"

/* Reset of Obj9C74C8: clears the flag/value pairs and both vectors. */
void func_08817CD4(Obj9C74C8 *self)
{
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_19 = 0;
    self->field_20 = 0;
    self->field_1A = 0;
    self->field_24 = 0;
    func_089A34E0(self->field_00);
    func_089A34E0(self->field_0C);
}
