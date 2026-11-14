#include "menu/auto_region1.h"

/* Screen15A88: func_08A292A0 on its board, has the menu root clear its
 * flags, field_CB0 = 0. */
void func_08A12F54(Screen15A88 *self)
{
    func_08A292A0(self->work->board);
    func_08A1D8C0(D_08A23FF0);
    self->work->field_CB0 = 0;
}
