#include "battle/auto_region1.h"

/* Clears field_04 once both resources are released. */
int func_089E77B8(PartE7988 *self)
{
    if (self->field_08 == 0 && self->field_0C == 0) {
        self->field_04 = 0;
    }
    return 0;
}
