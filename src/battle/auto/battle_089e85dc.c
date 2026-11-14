#include "battle/auto_region1.h"

/* field_40 of an active PartE8D08, otherwise 0. */
void *func_089E85DC(PartE8D08 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_40;
    }
}
