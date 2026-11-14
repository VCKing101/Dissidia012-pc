#include "battle/auto_region4.h"

/* Part96380: whether field_04 is set (not -1) while active. */
unsigned char func_08A95C9C(Part96380 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04 != -1;
    }
}
