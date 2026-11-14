#include "menu/auto_region4.h"

/* ScreenB29CF8: destroys its heap and releases field_134. */
void func_08B28C6C(ScreenB29CF8 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
