#include "menu/auto_region1.h"

/* ScreenE75C4: frees field_F7C from its heap and clears F70..F7C. */
void func_089E38D4(ScreenE75C4 *self)
{
    func_0885C538(self->heap, self->work->field_F7C, 0);
    self->work->field_F70 = 0;
    self->work->field_F74 = 0;
    self->work->field_F78 = 0;
    self->work->field_F7C = 0;
}
