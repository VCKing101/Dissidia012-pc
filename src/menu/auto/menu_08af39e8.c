#include "menu/auto_region3.h"

/* ScreenB01CF8: makes a text from the messages (any kind). */
void func_08AF39E8(ScreenB01CF8 *self, int kind, int arg1, int arg2, int arg3, int arg4)
{
    func_0887B3EC(self->work->messages, kind, arg1, arg2, arg3, arg4);
}
