#include "main/auto_region4.h"

/* The sum of the counters, field_58 counted 16 times. */
int func_088DC7A8(Obj8DC7A8 *self)
{
    return self->field_3C + self->field_40 + self->field_44 +
           self->field_48 + self->field_4C + self->field_50 +
           self->field_58 * 0x10;
}
