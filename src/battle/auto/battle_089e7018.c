#include "battle/auto_region1.h"

/* field_04 of an active PartE7988, otherwise 0. */
unsigned int func_089E7018(PartE7988 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_04;
    }
}
