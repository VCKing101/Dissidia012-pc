#include "menu/auto_region2.h"

/* Screen4FFF4: destroys its heap and releases field_38. */
void func_08A4C3A4(Screen4FFF4 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_38);
    self->field_38 = 0;
}
