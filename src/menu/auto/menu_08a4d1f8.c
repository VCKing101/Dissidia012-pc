#include "menu/auto_region2.h"

/* Screen4FFF4: sets the name the input writes back to. */
void func_08A4D1F8(Screen4FFF4 *self, Name66 *name)
{
    self->work->field_6E8C = name;
}
