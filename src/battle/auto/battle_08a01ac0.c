#include "battle/auto_region2.h"

/* field_08 when active, otherwise 0. */
void *func_08A01AC0(Part01DA8 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_08;
    }
}
