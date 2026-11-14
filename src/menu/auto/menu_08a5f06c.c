#include "menu/auto_region2.h"

/* Panel5FA00 reset. */
void func_08A5F06C(Panel5FA00 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    self->field_2C = 0;
    for (i = 0; i < 3; i++) {
        self->field_30[i] = 0;
    }
    for (i = 0; i < 0xB9; i++) {
        self->handles[i] = 0;
    }
    self->field_320 = 0;
    self->field_37C = 0;
    self->field_384 = 0;
}
