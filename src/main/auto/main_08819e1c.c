#include "main/auto_region1.h"

/* Active with field_01 clear. */
unsigned char func_08819E1C(Obj9C74E8 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_01 == 0;
    }
}
