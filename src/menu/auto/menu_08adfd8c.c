#include "menu/auto_region3.h"

/* Screen850: closes (func_08ADFF7C) and enters the next state; returns 0. */
int func_08ADFD8C(Screen850 *self)
{
    func_08ADFF7C(self);
    self->work->state = self->work->next;
    return 0;
}
