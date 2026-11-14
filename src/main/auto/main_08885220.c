#include "main/auto_region2.h"

/* Releases field_00 through D_089D69A8 and deletes the four parts. */
void func_08885220(Obj885220 *self)
{
    if (self->field_00 != 0) {
        func_08882298(D_089D69A8, self->field_00);
        self->field_00 = 0;
    }
    if (self->field_10 != 0) {
        func_088842E0(self->field_10, 3);
        self->field_10 = 0;
    }
    if (self->field_04 != 0) {
        func_08884A38(self->field_04, 3);
        self->field_04 = 0;
    }
    if (self->field_0C != 0) {
        func_08886208(self->field_0C, 3);
        self->field_0C = 0;
    }
    if (self->field_08 != 0) {
        func_08885DAC(self->field_08, 3);
        self->field_08 = 0;
    }
}
