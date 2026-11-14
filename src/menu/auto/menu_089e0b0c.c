#include "menu/auto_region1.h"

/* ScreenE75C4 constructor: builds the task, then sets up the rest. */
ScreenE75C4 *func_089E0B0C(ScreenE75C4 *self)
{
    func_08864FF0(&self->task, 1, 0x11177, 0x53);
    func_089E1BB0(self);
    return self;
}
