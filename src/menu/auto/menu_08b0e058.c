#include "menu/auto_region4.h"

/* ScreenB1CB00: the picked cell (column + row * 5). */
int func_08B0E058(ScreenB1CB00 *self)
{
    return self->work->field_24790.index + self->work->field_247BC.index * 5;
}
