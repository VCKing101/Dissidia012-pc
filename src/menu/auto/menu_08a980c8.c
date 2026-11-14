#include "menu/auto_region3.h"

/* ScreenA3F44: frees its heaps and blocks. */
void func_08A980C8(ScreenA3F44 *self)
{
    func_0885C488(self->field_140);
    self->field_140 = 0;
    func_0885C538(D_089D5EC0->field_710, self->field_13C, 0);
    self->field_13C = 0;
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->field_134);
    self->field_134 = 0;
}
