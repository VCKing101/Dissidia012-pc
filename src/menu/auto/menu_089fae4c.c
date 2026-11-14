#include "menu/auto_region1.h"

/* Screen0B6A0: opens its prompt with value. */
void func_089FAE4C(Screen0B6A0 *self, int value)
{
    func_088D4E00(value, &self->work->prompt);
}
