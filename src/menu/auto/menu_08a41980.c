#include "menu/auto_region2.h"

/* MenuRoot2: opens its prompt with value. */
void func_08A41980(MenuRoot2 *self, int value)
{
    func_088D4E00(value, &self->work->prompt);
}
