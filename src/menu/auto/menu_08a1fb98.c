#include "menu/auto_region1.h"

/* Menu root: resets fields 25C88..25C98. */
void func_08A1FB98(MenuRoot *self)
{
    self->work->field_25C88 = 0;
    self->work->field_25C8C = 0;
    self->work->field_25C90 = 0;
    self->work->field_25C98 = -1;
}
