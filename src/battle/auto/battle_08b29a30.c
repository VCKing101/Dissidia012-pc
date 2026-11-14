#include "battle/auto_region5.h"

/* Part29ED8 release: releases the members and both resources and
 * resets. */
void func_08B29A30(Part29ED8 *self)
{
    func_08B29FF0(&self->field_58);
    func_08B2BBC0(&self->field_34);
    func_08B2C334(&self->field_0C);
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08B29A14(self);
}
