#include "menu/auto_region2.h"

/* Panel69A78 reset. */
void func_08A6963C(Panel69A78 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    self->field_2C = 0;
    for (i = 0; i < 1; i++) {
        self->field_44[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        self->texts[i] = 0;
    }
    self->messages = 0;
    self->field_34 = -1;
    self->field_38 = -1;
    self->field_3C = -1;
    self->field_40 = -1;
}
