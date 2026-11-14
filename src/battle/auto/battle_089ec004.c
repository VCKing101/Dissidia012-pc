#include "battle/auto_region1.h"

/* True when inactive or when field_38 is 1.0. */
unsigned char func_089EC004(PartECED0 *self)
{
    if (!self->active) {
        return 1;
    } else {
        return self->field_38 == 1.0f;
    }
}
