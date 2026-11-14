#include "menu/auto_region2.h"

/* Panel61C40 reset. */
void func_08A60768(Panel61C40 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    self->field_2C = 0;
    self->messages = 0;
    for (i = 0; i < 3; i++) {
        self->field_34[i] = 0;
    }
    for (i = 0; i < 0x19; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        self->texts[i] = 0;
    }
    self->field_124 = 0;
    self->data = 0;
    self->field_12C = 0;
}
