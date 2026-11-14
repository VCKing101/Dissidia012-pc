#include "menu/auto_region3.h"

/* Screen8B7A8: runs func_08A89F40 and sets work->field_418 to 4. */
void func_08A89200(Screen8B7A8 *self)
{
    func_08A89F40(self);
    self->work->field_418 = 4;
}
