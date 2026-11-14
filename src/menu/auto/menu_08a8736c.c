#include "menu/auto_region3.h"

/* Screen87EAC: opens the prompt with a value. */
void func_08A8736C(Screen87EAC *self, int value)
{
    func_088D4E00(value, &self->work->prompt);
    func_08A873A8(self, 1);
}
