#include "menu/auto_region2.h"

/* Panel6C638 reset. */
void func_08A6B4C4(Panel6C638 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    self->field_2C = 0;
    for (i = 0; i < 0x1D; i++) {
        self->field_34[i] = 0;
    }
    for (i = 0; i < 0x3A; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 0x15; i++) {
        self->texts[i] = 0;
    }
    self->field_30 = 0;
    self->data = 0;
    self->field_214 = -1;
    func_08A574CC(&self->view);
}
