#include "menu/agent20_types.h"

/* Counts up the shown total (2 on the first step, at most 999), keeps the record and marks it shown. */
extern short func_089F0230(A20EntryOwner *self);

void func_089F2E7C(A20EntryOwner *self)
{
    if ((self->field_14C->field_279C & 1) != 0) {
        if (self->field_14C->field_2728 == 0) {
            self->field_14C->field_2728 = 2;
        } else {
            self->field_14C->field_2728++;
            if (self->field_14C->field_2728 >= 1000) {
                self->field_14C->field_2728 = 999;
            }
        }
        if (self->field_14C->field_27A2 < self->field_14C->field_2728) {
            self->field_14C->field_27A2 = self->field_14C->field_2728;
        }
        self->field_14C->field_2744 = func_089F0230(self);
    }
    self->field_14C->field_279C |= 1;
}
