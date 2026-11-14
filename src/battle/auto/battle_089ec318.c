#include "battle/auto_region1.h"

/* field_09 of the member at +0x44 when active, otherwise 0. */
unsigned char func_089EC318(PartECED0 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_44.field_09;
    }
}
