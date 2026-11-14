#include "menu/auto_region2.h"

/* Panel5EED8 reset. */
void func_08A5DE54(Panel5EED8 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    for (i = 0; i < 4; i++) {
        self->field_30[i] = 0;
    }
    for (i = 0; i < 0x11; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 7; i++) {
        self->texts[i] = 0;
    }
    self->messages = 0;
    self->field_150 = 0;
    func_08A601EC(&self->field_A0);
    self->field_154 = 0;
    self->field_160 = 0;
    self->field_170 = 1;
    self->field_168 = 0;
    self->field_16C = -1;
    self->field_171 = 0;
}
