#include "menu/auto_region2.h"

/* Screen4C0FC: destroys its heap and releases field_134. */
void func_08A49CB0(Screen4C0FC *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
