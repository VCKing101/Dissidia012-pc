#include "menu/auto_region1.h"

/* ScreenE75C4: clears field_F68 (the forwarder func_089E0AE4 passes two
 * more arguments, unused here). */
void func_089E2D4C(ScreenE75C4 *self, int arg1, int arg2)
{
    self->work->field_F68 = 0;
}
