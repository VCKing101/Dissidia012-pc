#include "battle/auto_region4.h"

/* Part96380 start: resets it and marks it active. */
int func_08A95890(Part96380 *self)
{
    func_08A95874(self);
    self->active = 1;
    return 0;
}
