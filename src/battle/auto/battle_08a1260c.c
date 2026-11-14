#include "battle/auto_region2.h"

/* When func_08A22E2C reports a result for (field_2B4, field_2B8), signals field_1C and moves field_08 to 3. */
int func_08A1260C(Unit12A88 *self)
{
    char info[44];

    if (func_08A22E2C(info, self->field_2B4, self->field_2B8, 0xD) != 0) {
        func_08A01C74(&self->field_1C, 9);
        func_08A16C58(self->field_44);
        self->field_08 = 3;
    }
    return 0;
}
