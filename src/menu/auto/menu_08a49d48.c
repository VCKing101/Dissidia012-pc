#include "menu/auto_region2.h"

/* Screen4C0FC: makes a text from its messages (kind 0). */
void func_08A49D48(Screen4C0FC *self, int arg1, int arg2, int arg3, int arg4)
{
    func_0887B3EC(self->work->messages, 0, arg1, arg2, arg3, arg4);
}
