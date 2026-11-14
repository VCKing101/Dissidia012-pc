#include "menu/auto_region1.h"

/* Screen15A88: shuts its board down, frees its block and heap and frees
 * field_EF8 from D_089D5EC0's heap. */
void func_08A12E34(Screen15A88 *self)
{
    func_08A28C24(self->work->board);
    func_08A1C07C(D_08A23FF0);
    func_0885C538(self->work->field_EFC, self->work->field_EC8, 0);
    self->work->board = 0;
    func_0885C488(self->work->field_EFC);
    func_0885C538(D_089D5EC0->field_710, self->work->field_EF8, 0);
}
