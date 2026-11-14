#include "menu/auto_region1.h"

/* Menu root: destroys its heap and releases field_134. */
void func_08A1B394(MenuRoot *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
