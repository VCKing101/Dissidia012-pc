#include "menu/auto_region3.h"

/* ScreenA3F44: opens the prompt with a value. */
void func_08A9C55C(ScreenA3F44 *self, int value)
{
    func_088D4E00(value, &self->work->prompt);
    func_08A9C5A0(self, 1);
}
