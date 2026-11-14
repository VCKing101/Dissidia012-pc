#include "menu/auto_region4.h"

void func_08B4C0B4(Widget4C010 *self)
{
    int i;

    func_0892AC2C(self);
    self->state = 0;
    self->id = 0;
    for (i = 0; i < 2; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->texts[i] = 0;
    }
    self->field_40 = 0;
    self->field_44 = 0;
    self->field_48 = 0;
    self->field_4C = 0;
    self->field_50 = 0;
    self->field_54 = 0;
}
