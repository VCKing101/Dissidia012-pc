#include "menu/auto_region1.h"

/* Screen38EA8: unless field_6C is 1, clears field_DB5/field_DB6 and redraws. */
void func_08A37504(Screen38EA8 *self)
{
    if (self->work->field_6C != 1) {
        self->work->field_DB5 = 0;
        self->work->field_DB6 = 0;
        func_08A37E10(self, 0);
        func_08A371E4(self);
    }
}
