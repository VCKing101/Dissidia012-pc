#include "battle/auto_region4.h"

/* Part815C8: whether field_54 equals what func_08A87AF8 reports for
 * field_0C. */
unsigned char func_08A812DC(Part815C8 *self)
{
    int value;

    value = func_08A87AF8(self->field_0C);
    return self->field_54 == value;
}
