#include "menu/auto_region1.h"

/* Screen0B6A0: makes a text from its messages (kind 0). */
void *func_089EF1E8(Screen0B6A0 *self, int arg1, int arg2, int arg3, int arg4)
{
    return func_0887B3EC(self->work->messages, 0, arg1, arg2, arg3, arg4);
}
