#include "battle/auto_region2.h"

/* field_11 when active, otherwise 0. */
unsigned char func_08A0775C(Part08158 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_11;
    }
}
