#include "menu/auto_region2.h"

/* Panel58420 reset: clears its arrays and points items[] at its members. */
void func_08A574CC(Panel58420 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->field_28 = 0;
    self->state = 0;
    self->field_30 = 0;
    for (i = 0; i < 0x27; i++) {
        self->field_34[i] = 0;
    }
    for (i = 0; i < 5; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        self->texts[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        self->items[i] = 0;
    }
    self->field_10EC = 0;
    self->field_1120 = 0;
    self->items[0] = &self->field_100.base;
    self->items[1] = &self->field_33C.base;
    self->items[2] = &self->field_AB0.base;
    self->items[3] = &self->field_578.base;
    self->items[4] = &self->field_DFC.base;
    self->items[5] = &self->field_F54.base;
}
