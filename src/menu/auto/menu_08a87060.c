#include "menu/auto_region3.h"

/* Screen87EAC: deletes work->field_920 and frees work->field_91C. */
void func_08A87060(Screen87EAC *self)
{
    func_08A80D40(self->work->field_920, 2);
    func_0885C538(self->heap, self->work->field_91C, 0);
}
