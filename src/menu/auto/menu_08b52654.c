#include "menu/auto_region4.h"

void func_08B52654(Widget525B0 *self)
{
    int i;

    func_0892AC2C(self);
    self->state = 0;
    self->id = 0;
    for (i = 0; i < 3; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->texts[i] = 0;
    }
    self->field_44 = 1;
    self->field_45 = 0;
    self->field_48 = 0;
    self->field_4C = 0;
    self->field_50 = 0;
}
