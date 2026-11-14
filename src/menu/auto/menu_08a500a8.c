#include "menu/auto_region2.h"

/* Screen51CBC constructor: builds its task and resets it. */
Screen51CBC *func_08A500A8(Screen51CBC *self)
{
    func_08864FF0(&self->task, 1, 0x11180, 0x4C);
    func_08A50118(self);
    return self;
}
