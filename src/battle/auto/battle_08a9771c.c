#include "battle/auto_region4.h"

/* Part98058: field_34 while active, otherwise 0. */
unsigned char func_08A9771C(Part98058 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_34;
    }
}
