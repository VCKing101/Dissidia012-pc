#include "battle/auto_region5.h"

/* PartFBE40 release: releases the members, both handles, field_30 and the
 * model at field_3C. */
void func_08AFBF70(PartFBE40 *self)
{
    int i;

    func_088EC544(self->field_04);
    func_088EC2F8(self->field_18);
    for (i = 0; i < 2; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
    if (self->field_30 != 0) {
        func_088DAB44(self->field_30);
        self->field_30 = 0;
    }
    if (self->field_3C != 0) {
        func_0887BA5C(self->field_3C);
        self->field_3C = 0;
    }
}
