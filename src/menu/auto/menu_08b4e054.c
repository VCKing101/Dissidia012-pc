#include "menu/auto_region4.h"

void func_08B4E054(Widget4DF58 *self)
{
    int i;

    func_0892AC2C(self);
    self->state = 0;
    for (i = 0; i < 2; i++) {
        self->field_2C[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        self->handles[i] = 0;
    }
    self->field_38 = 0;
    self->field_3C = 0;
    func_08931EB8(self->field_40);
    func_08B4CAD4(&self->field_64);
    func_08B4CF18(&self->field_84);
    func_08B4D518(&self->field_D8);
}
