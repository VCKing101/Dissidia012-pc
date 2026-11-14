#include "menu/auto_region3.h"

/* ScreenC4560: makes a text from the messages. */
void *func_08ABBC4C(ScreenC4560 *self, int arg1, int arg2, int arg3, int arg4)
{
    return func_0887B3EC(self->work->messages, 0, arg1, arg2, arg3, arg4);
}
