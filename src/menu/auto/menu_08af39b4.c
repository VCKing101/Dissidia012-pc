#include "menu/auto_region3.h"

/* ScreenB01CF8: makes a text from the messages. */
void func_08AF39B4(ScreenB01CF8 *self, int arg1, int arg2, int arg3, int arg4)
{
    func_0887B3EC(self->work->messages, 0, arg1, arg2, arg3, arg4);
}
