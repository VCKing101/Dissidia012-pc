#include "battle/auto_region1.h"

/* field_48 of an active PartE8D08, otherwise 0. */
int func_089E85BC(PartE8D08 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_48;
    }
}
