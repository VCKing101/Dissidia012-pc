#include "menu/auto_region2.h"

/* Item: clears field_F8, then func_08A557D0 and func_08A55878. */
void func_08A55070(Item *self)
{
    self->field_F8 = 0;
    func_08A557D0(self);
    func_08A55878(self);
}
