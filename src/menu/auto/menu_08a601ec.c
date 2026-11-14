#include "menu/auto_region2.h"

/* Panel60670 reset. */
void func_08A601EC(Panel60670 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    for (i = 0; i < 1; i++) {
        self->field_34[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->texts[i] = 0;
    }
    self->messages = 0;
    self->field_30 = -1;
}
