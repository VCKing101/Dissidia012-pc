#include "menu/auto_region1.h"

/* Screen35A00: func_08A31438; while field_28 is 0, func_08A3210C and swaps
 * flag 2 of field_174 for 4. */
void func_08A313D8(Screen35A00 *self)
{
    func_08A31438(self);
    if (self->work->field_28 == 0) {
        func_08A3210C(self);
        self->work->field_174 = self->work->field_174 & ~2;
        self->work->field_174 = self->work->field_174 | 4;
    }
}
