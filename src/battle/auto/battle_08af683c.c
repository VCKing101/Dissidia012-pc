#include "battle/auto_region5.h"

/* PartF66C0: moves to phase 4 once field_74 has finished. */
void func_08AF683C(PartF66C0 *self)
{
    if (func_08AF3DD4(&self->field_74) == 1) {
        self->phase = 4;
    }
}
