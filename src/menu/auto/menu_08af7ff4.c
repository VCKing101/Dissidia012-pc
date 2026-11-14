#include "menu/auto_region4.h"

/* ScreenB01CF8: returns field_B64 to the heap. */
void func_08AF7FF4(ScreenB01CF8X *self)
{
    func_0885C538(self->heap, self->work->field_B64, 0);
    self->work->field_B64 = 0;
}
