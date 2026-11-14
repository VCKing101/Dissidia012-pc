#include "main/auto_region1.h"

/* For kind 1 the flag byte of entry index in field_10, otherwise whether
 * slot index of field_08 is set. */
unsigned char func_0881A9C8(Obj9C7510 *self, Ref81A9C8 *ref)
{
    unsigned char result;

    if (ref->kind == 1) {
        result = self->field_10[ref->index].field_08;
    } else {
        result = self->field_08[ref->index] != 0;
    }
    return result;
}
