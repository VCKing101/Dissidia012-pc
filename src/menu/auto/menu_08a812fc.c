#include "menu/auto_region3.h"

/* Screen7EEDC: mode 2; reads two values with func_08A7F104 and sets flag 4. */
void func_08A812FC(Screen7EEDC *self)
{
    unsigned int value;

    self->work->field_608 = 2;
    self->work->field_60C = 0;
    value = func_08A7F104(self, 2, -1);
    self->work->field_5FC = value;
    value = func_08A7F104(self, 1, -1);
    self->work->field_600 = value;
    self->work->flags = self->work->flags | 4;
}
