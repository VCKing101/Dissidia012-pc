#include "battle/auto_region5.h"

/* Releases the handles at field_14, field_10 and field_0C and runs
 * func_0885631C on D_089D4DC8's field_174. */
void func_08B503C8(Part4EE2C *self)
{
    func_0899BB1C(self->field_14);
    self->field_14 = 0;
    func_0899BB1C(self->field_10);
    self->field_10 = 0;
    func_0899BB1C(self->field_0C);
    self->field_0C = 0;
    func_0885631C(D_089D4DC8->field_174);
}
