#include "menu/auto_region3.h"

/* ScreenC47C4: frees the heap. */
void func_08AC647C(ScreenC47C4 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
}
