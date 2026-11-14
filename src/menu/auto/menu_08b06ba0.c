#include "menu/auto_region4.h"

unsigned char func_08B06BA0(ScreenB1CB00 *self)
{
    return (self->work->field_1F480 & 0x800) != 0;
}
