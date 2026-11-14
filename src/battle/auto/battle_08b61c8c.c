#include "battle/auto_region5.h"

/* Part61C08: releases the handle at field_10 and field_0C and turns
 * D_089D4DC8's field_7180 off. */
void func_08B61C8C(Part61C08 *self)
{
    func_0899BB1C(self->field_10);
    self->field_10 = 0;
    if (self->field_0C != 0) {
        func_0886EB84(self->field_0C);
        self->field_0C = 0;
    }
    func_0885BA0C(D_089D4DC8->field_7180, 0);
}
