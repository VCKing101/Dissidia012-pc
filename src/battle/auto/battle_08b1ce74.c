#include "battle/auto_region5.h"

/* Part1D810 release: releases and frees field_68 and field_64, releases
 * field_10 and resets. */
void func_08B1CE74(Part1D810 *self)
{
    if (self->heap != 0) {
        if (self->field_68 != 0) {
            func_08B1C2FC(self->field_68);
            func_0885C538(self->heap, self->field_68, 0);
            self->field_68 = 0;
        }
        if (self->field_64 != 0) {
            func_08B1A458(self->field_64);
            func_0885C538(self->heap, self->field_64, 0);
            self->field_64 = 0;
        }
    }
    if (self->field_10 != 0) {
        func_088DAB44(self->field_10);
        self->field_10 = 0;
    }
    func_08B1CE50(self);
}
