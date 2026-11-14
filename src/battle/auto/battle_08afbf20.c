#include "battle/auto_region5.h"

/* PartFBE40: sets up handles[0] and the two members from `setup`. */
void func_08AFBF20(PartFBE40 *self, SetupFBF20 *setup)
{
    self->handles[0] = func_088DAA68(setup->field_18);
    func_088EC500(self->field_04, setup->field_08);
    func_088EC2A8(self->field_18, setup->field_1C);
}
