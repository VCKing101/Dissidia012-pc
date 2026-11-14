#include "menu/auto_region1.h"

/* Screen0B6A0: destroys its heap and releases field_134. */
void func_089EF150(Screen0B6A0 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
