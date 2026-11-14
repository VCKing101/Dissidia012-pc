#include "menu/auto_region3.h"

/* Screen8B7A8: frees its heaps and blocks. */
void func_08A89020(Screen8B7A8 *self)
{
    int busy;

    busy = func_08867894();
    if (busy == 0) {
        func_0885C488(self->field_140);
        self->field_140 = 0;
        func_0885C538(D_089D5EC0->field_710, self->field_13C, 0);
        self->field_13C = 0;
    }
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
