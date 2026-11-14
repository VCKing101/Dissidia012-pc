#include "menu/auto_region3.h"

/* ScreenC4560 constructor: sets up the task (id 0x1117A). */
ScreenC4560 *func_08ABF6F8(ScreenC4560 *self)
{
    func_08864FF0(&self->task, 1, 0x1117A, 0x4C);
    return self;
}
