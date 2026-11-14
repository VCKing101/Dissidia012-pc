#include "menu/auto_region1.h"

/* Screen15A88: func_08A28EC0 on its board, if it has one. */
void func_08A12E08(Screen15A88 *self)
{
    if (self->work->board != 0) {
        func_08A28EC0(self->work->board);
    }
}
