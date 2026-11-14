#include "menu/auto_region3.h"

/* ScreenAB7CFC: starts the 30-frame countdown (flag 0x400). */
void func_08AAB170(ScreenAB7CFC *self)
{
    self->work->field_7CDC = 0x1E;
    self->work->flags = self->work->flags | 0x400;
}
