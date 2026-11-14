#include "menu/auto_region3.h"

/* ScreenC4560: frees the heap and field_138. */
void func_08ABF054(ScreenC4560 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    if (self->field_138 != 0) {
        func_0888208C(self->field_138);
        self->field_138 = 0;
    }
}
