#include "battle/auto_region2.h"

/* Releases the part and marks it active again. */
int func_08A113A8(Part11640 *self)
{
    func_08A11350(self);
    self->active = 1;
    return 0;
}
