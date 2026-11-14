#include "menu/auto_region1.h"

/* ScreenE75C4: destroys its heap and releases field_134. */
void func_089E108C(ScreenE75C4 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
