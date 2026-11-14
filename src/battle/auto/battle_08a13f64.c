#include "battle/auto_region2.h"

/* When func_08A22E2C reports a result for (field_1F4, field_1F8), signals field_24 and moves field_08 to 3. */
int func_08A13F64(Unit14458 *self)
{
    char info[44];

    if (func_08A22E2C(info, self->field_1F4, self->field_1F8, 0xE) != 0) {
        func_08A01C74(&self->field_24, 5);
        func_08A16C58(self->field_4C);
        self->field_08 = 3;
    }
    return 0;
}
