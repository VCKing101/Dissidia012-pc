#include "battle/auto_region1.h"

/* Starts over: releases, sets up the member at +4 and marks the part active. */
int func_089F64AC(PartF65E0 *self)
{
    func_089F6490(self);
    func_08AEAF84(self->field_04, 0, 2, 10, 0, -1);
    self->active = 1;
    return 0;
}
