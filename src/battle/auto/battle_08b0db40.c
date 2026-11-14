#include "battle/auto_region5.h"

/* Part0DB40 release: destroys field_90, releases the members and resets. */
void func_08B0DB40(Part0DB40 *self)
{
    if (self->field_90 != 0) {
        func_08B0E63C(self->field_90, 3);
        self->field_90 = 0;
    }
    func_08B14484(self->field_08);
    func_08B121DC(self->field_A0);
    func_08B0E4D4(self);
}
