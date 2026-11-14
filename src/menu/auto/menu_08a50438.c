#include "menu/auto_region2.h"

/* Screen51CBC: destroys its heap and releases field_0C. */
void func_08A50438(Screen51CBC *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_0C);
    self->field_0C = 0;
}
