#include "battle/auto_region2.h"

/* Part08158 deleting destructor. */
void func_08A06C18(Part08158 *self, int flags)
{
    if (self) {
        self->vtable = D_08A08158;
        func_08A06D38(self);
        func_0899372C(self->field_224, 2, sizeof(Part048C8), func_08A038B8, 0, 0);
        func_0899372C(self->field_1CC, 2, sizeof(Part10F98), func_08A10B00, 0, 0);
        func_0899372C(self->field_164, 2, sizeof(Part06B08), func_08A061D0, 0, 0);
        func_089F5750(&self->field_5C, 2);
        func_089F0EE8(&self->field_48, 2);
        func_08A112E0(&self->field_34, 2);
        func_08A15578(self->field_28, 2);
        func_08A159A0(self->field_20, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
