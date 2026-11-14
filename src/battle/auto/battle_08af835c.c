#include "battle/auto_region5.h"

/* PartF820C: moves to phase 4 once field_00 has finished. */
void func_08AF835C(PartF820C *self)
{
    if (func_08AF3DD4(&self->field_00) == 1) {
        self->phase = 4;
    }
}
