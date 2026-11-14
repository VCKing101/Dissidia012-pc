#include "menu/auto_region3.h"

/* Screen8B7A8 constructor: sets up the task (id 0x11189). */
Screen8B7A8 *func_08A8A194(Screen8B7A8 *self)
{
    func_08864FF0(&self->task, 1, 0x11189, 0x4C);
    return self;
}
