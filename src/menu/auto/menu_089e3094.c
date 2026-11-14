#include "menu/auto_region1.h"

/* ScreenE75C4: opens its prompt with value. */
void func_089E3094(ScreenE75C4 *self, int value)
{
    func_088D4E00(value, &self->work->prompt);
}
