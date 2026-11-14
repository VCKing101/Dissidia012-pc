#include "menu/auto_region1.h"

/* ScreenE75C4: makes a text from its messages (kind 0). */
void func_089E2AB8(ScreenE75C4 *self, int arg1, int arg2, int arg3, int arg4)
{
    func_0887B3EC(self->work->messages, 0, arg1, arg2, arg3, arg4);
}
