#include "menu/auto_region2.h"

/* Screen457F0: destroys its heap and releases field_138. */
void func_08A43628(Screen457F0 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_138);
    self->field_138 = 0;
}
