#include "menu/auto_region2.h"

/* Panel63680: scrolls to row field_D90 (at most the last), then redraws. */
void func_08A62C58(Panel63680 *self)
{
    self->scroll.field_04 = self->field_D90;
    if (self->scroll.field_0C < self->field_D90) {
        self->scroll.field_04 = self->scroll.field_0C;
    }
    func_08A62E14(self);
    func_08A62D9C(self);
    func_08A62CAC(self);
}
