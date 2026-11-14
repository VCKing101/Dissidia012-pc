#include "battle/auto_region5.h"

/* PartFBE40: without field_24, runs the field_04 member (phase 3);
 * otherwise loads handles[1] from field_30 and flags D_089D5EC0 (phase 6). */
void func_08AFC090(PartFBE40 *self)
{
    if (self->field_24 == 0) {
        func_088EC658(self->field_04);
        func_088EC67C(self->field_04);
        self->phase = 3;
    } else {
        self->handles[1] = func_088DAA40(self->field_30, 0, 0);
        D_089D5EC0->field_2CD1 = 1;
        self->field_40 = 0;
        self->phase = 6;
    }
}
