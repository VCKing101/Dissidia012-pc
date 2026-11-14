#include "menu/auto_region2.h"

/* Page67EE0 reset. */
void func_08A63A84(Page67EE0 *self)
{
    int i;

    func_08A638C4(&self->base);
    self->base.field_03 = 1;
    self->field_10 = 0;
    self->field_14 = 0;
    for (i = 0; i < 6; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        self->texts[i] = 0;
    }
    self->field_18 = 0;
    self->field_A0 = 0;
}
