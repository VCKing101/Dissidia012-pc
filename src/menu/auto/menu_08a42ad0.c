#include "menu/auto_region2.h"

/* MenuRoot2: destroys its heap and releases field_138. */
void func_08A42AD0(MenuRoot2 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_138);
    self->field_138 = 0;
}
