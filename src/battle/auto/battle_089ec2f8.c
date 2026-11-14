#include "battle/auto_region1.h"

/* field_08 of the member at +0x44 when active, otherwise 0. */
unsigned char func_089EC2F8(PartECED0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_44.field_08;
    }
}
