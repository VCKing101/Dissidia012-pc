#include "menu/auto_region4.h"

void func_08B4D518(Part4D4DC *self)
{
    int i;

    func_08931DA0(self);
    for (i = 0; i < 0xC; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 0x13; i++) {
        self->texts[i] = 0;
    }
    self->field_8C = 0;
    self->field_90 = 0;
    self->field_94 = 0;
    self->field_98 = 0;
    self->field_9C = -1;
    self->field_A2 = 0;
}
