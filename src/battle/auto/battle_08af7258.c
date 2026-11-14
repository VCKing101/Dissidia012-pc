#include "battle/auto_region5.h"

/* PartF6A34: moves to phase 4 once field_160 has finished. */
void func_08AF7258(PartF6A34 *self)
{
    if (func_08AF3DD4(&self->field_160) == 1) {
        self->phase = 4;
    }
}
