#include "battle/auto_region4.h"

/* Part98058: field_18 while active, otherwise 0. */
unsigned int func_08A97598(Part98058 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_18;
    }
}
