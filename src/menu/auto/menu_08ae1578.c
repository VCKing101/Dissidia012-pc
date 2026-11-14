#include "menu/auto_region3.h"

/* Screen850: field_624 at func_08AE150C's index. */
int func_08AE1578(Screen850 *self)
{
    int index;

    index = func_08AE150C(self);
    return self->work->field_624[index];
}
