#include "battle/auto_region4.h"

/* Part98058: field_2C while active, otherwise 0. */
unsigned int func_08A976C4(Part98058 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_2C;
    }
}
