#include "menu/auto_region1.h"

/* ScreenE75C8: makes a text from its messages (kind 3). */
void func_08A2BA64(ScreenE75C8 *self, int arg1, int arg2, int arg3)
{
    func_0887B3EC(self->work->messages, 3, 0, arg1, arg2, arg3);
}
