#include "menu/auto_region2.h"

/* Panel5EED8: counts field_18 up to 2, then closes (state 0). */
void func_08A5E5F4(Panel5EED8 *self)
{
    if (self->base.field_18 < 2) {
        self->base.field_18 = self->base.field_18 + 1;
    } else {
        func_08A4DE34(D_08A4FFF4);
        self->state = 0;
    }
}
