#include "menu/auto_region3.h"

/* ScreenC4560: message id from the messages. */
char *func_08ABBC80(ScreenC4560 *self, int id)
{
    return func_0887B500(self->work->messages, 0, id);
}
