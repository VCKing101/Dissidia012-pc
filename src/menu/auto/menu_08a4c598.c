#include "menu/auto_region2.h"

/* Screen4FFF4: func_0887E684(D_089D6998), frees work field_08. */
void func_08A4C598(Screen4FFF4 *self)
{
    func_0887E684(D_089D6998);
    func_0885C538(self->heap, self->work->field_08, 0);
    self->work->field_08 = 0;
}
