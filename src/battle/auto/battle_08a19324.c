#include "battle/auto_region3.h"

/* Unit1A350 deleting destructor. */
void func_08A19324(Unit1A350 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08A1A350;
        func_08A19434(self);
        func_0899372C(self->field_F4, 2, sizeof(Part17C90), func_08A17540, 0, 0);
        func_0899372C(self->field_AC, 2, sizeof(Part02198), func_08A01E08, 0, 0);
        func_0899372C(self->field_64, 2, sizeof(Part19278), func_08A18D80, 0, 0);
        func_0899372C(self->field_14, 2, sizeof(Part01DA8), func_08A017C0, 0, 0);
        func_089F1304(&self->base, 0);
        if (flags & 1) {
            func_089F1354(self);
        }
    }
}
