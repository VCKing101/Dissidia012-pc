#include "menu/auto_region4.h"

void func_08B358EC(ScreenB41CC0 *self)
{
    func_08B37AA4(self);
    if (self->field_B4 == 5) {
        func_08B40590(self);
        func_08B39C9C(self, 1);
        self->field_AC = 3;
    }
}
