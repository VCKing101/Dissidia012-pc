#include "battle/auto_region4.h"

/* PartBBDE8 deleting destructor: releases, then destroys the members in
 * reverse order. */
void func_08ABAD30(PartBBDE8 *self, int flags)
{
    if (self) {
        self->vtable = D_08ABBDE8;
        func_08ABAE94(self);
        func_08ABAE78(self);
        func_08ABAE5C(self);
        func_08ABAE40(self);
        func_08AB7418(&self->field_1D8, 2);
        func_08AB75A8(&self->field_1C8, 2);
        func_08AC0B08(&self->field_1B8, 2);
        func_08ABA040(&self->field_1A4, 2);
        func_08AC1EB0(&self->field_16C, 2);
        func_08ABD608(&self->field_15C, 2);
        func_08ABDEBC(&self->field_DC, 2);
        func_08ABA820(&self->field_B4, 2);
        func_08ABBFE8(&self->field_60, 2);
        func_08AB9A18(&self->field_00, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
