#include "menu/auto_region3.h"

/* ObjAB35B8: sets flag 0x20 and keeps func_089C5FB8's value. */
void func_08AB35B8(ObjAB35B8 *self)
{
    int value;

    self->work->flags = self->work->flags | 0x20;
    value = func_089C5FB8();
    self->work->field_4A8 = value;
}
