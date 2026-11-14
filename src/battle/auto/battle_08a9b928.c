#include "battle/auto_region4.h"

/* PartA1BD8: while field_04 is 0x14, raises field_C0 to at least field_B8
 * + 1. */
int func_08A9B928(PartA1BD8 *self)
{
    int next;

    if (self->field_04 == 0x14 && (next = self->field_B8 + 1, self->field_C0 < next)) {
        self->field_C0 = next;
    }
    return 0;
}
