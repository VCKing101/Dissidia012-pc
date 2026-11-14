#include "menu/auto_region1.h"

/* ScreenE75C8: whether field_14F4 is set. */
unsigned char func_08A2B680(ScreenE75C8 *self)
{
    return self->work->field_14F4 != 0;
}
