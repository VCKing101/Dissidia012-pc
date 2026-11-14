#include "menu/auto_region2.h"

/* Panel694A8 reset. */
void func_08A683C4(Panel694A8 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    self->field_2C = 2;
    self->field_28 = 2;
    self->field_34 = 0;
    for (i = 0; i < 2; i++) {
        self->field_38[i] = 0;
    }
    for (i = 0; i < 0x10; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        self->texts[i] = 0;
    }
    self->field_D0 = 0;
    self->field_D4 = 0;
    self->field_D8 = 0;
    self->field_DC = 0;
}
