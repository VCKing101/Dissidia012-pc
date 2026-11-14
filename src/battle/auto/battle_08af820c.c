#include "battle/auto_region5.h"

/* PartF820C: when field_44 is set, runs func_08AF839C and then either goes
 * straight to phase 4 or starts hiding (phase 2). */
void func_08AF820C(PartF820C *self)
{
    if (self->field_44 == 1) {
        func_08AF839C(self, 1);
        if (self->field_45 == 1 && self->field_24 == 1) {
            self->phase = 4;
        } else {
            func_08AF3CFC(&self->field_00);
            self->phase = 2;
        }
    }
}
