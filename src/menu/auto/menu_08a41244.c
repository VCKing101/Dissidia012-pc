#include "menu/auto_region2.h"

/* MenuRoot2: clears two globals' state and frees its two work blocks. */
void func_08A41244(MenuRoot2 *self)
{
    func_08A3BF9C();
    func_08A3BF2C();
    func_0885C538(self->heap, self->work->field_E38, 0);
    self->work->field_E38 = 0;
    func_0885C538(self->heap, self->work->field_E34, 0);
    self->work->field_E34 = 0;
}
