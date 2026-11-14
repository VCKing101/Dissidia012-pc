#include "menu/auto_region2.h"

/* Page680D8 reset. */
void func_08A6545C(Page680D8 *self)
{
    int i;

    func_08A638C4(&self->base);
    self->base.field_04 = 1;
    self->field_10 = 0;
    self->field_14 = 0;
    for (i = 0; i < 8; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        self->texts[i] = 0;
    }
    self->entry = 0;
    self->field_90 = 0;
}
