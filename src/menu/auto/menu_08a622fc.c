#include "menu/auto_region2.h"

/* Panel63680: keeps value and sets field_E44, then redraws. */
void func_08A622FC(Panel63680 *self, int value)
{
    self->field_E40 = value;
    func_08A631EC(self);
    self->field_E44 = 1;
    func_08A628E0(self);
    func_08A62A3C(self);
}
