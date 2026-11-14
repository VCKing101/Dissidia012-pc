#include "main/auto_region2.h"

/* Whether the value entry index points at equals field_B4. */
unsigned char func_08886A38(Obj886A38 *self, int index)
{
    return self->field_B4 == *self->field_00[index];
}
