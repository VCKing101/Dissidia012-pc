#include "menu/auto_region1.h"

/* ScreenE75C4 step: back to state 0 once func_0887066C(D_089D5FB4) is 0. */
void func_089E1B74(ScreenE75C4 *self)
{
    int busy;

    busy = func_0887066C(D_089D5FB4);
    if (busy == 0) {
        self->work->state = 0;
    }
}
