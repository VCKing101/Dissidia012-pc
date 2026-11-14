#include "menu/auto_region3.h"

/* ScreenB01CF8: frees the heap. */
void func_08AF2400(ScreenB01CF8 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
}
