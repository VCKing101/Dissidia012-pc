#include "menu/auto_region3.h"

/* ScreenBB9E4: frees the heap. */
void func_08AB9A38(ScreenBB9E4 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
}
