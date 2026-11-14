#include "menu/auto_region2.h"

/* Screen51CBC: func_0887E684(D_089D6998), frees work field_6B18. */
void func_08A504DC(Screen51CBC *self)
{
    func_0887E684(D_089D6998);
    func_0885C538(self->heap, self->work->field_6B18, 0);
    self->work->field_6B18 = 0;
}
