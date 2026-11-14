#include "menu/auto_region1.h"

/* ScreenE75C4: shuts down field_F84, then frees F84 and F80 from its heap. */
void func_089E3A34(ScreenE75C4 *self)
{
    func_088CB054(self->work->field_F84);
    func_088CAF90(self->work->field_F84);
    func_0885C538(self->heap, self->work->field_F84, 0);
    func_0885C538(self->heap, self->work->field_F80, 0);
}
