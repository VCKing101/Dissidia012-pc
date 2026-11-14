#include "battle/auto_region3.h"

/* field_4C while active, otherwise 0. */
unsigned char func_08A66404(Part663E4 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_4C;
    }
}
