#include "menu/auto_region1.h"

/* Screen0B6A0: clears field_3430 (the forwarder func_089FE668 passes two
 * more arguments, unused here). */
void func_089FB6FC(Screen0B6A0 *self, int arg1, int arg2)
{
    self->work->field_3430 = 0;
}
