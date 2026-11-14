#include "menu/auto_region3.h"

/* ScreenAB7CFC: true while handle 0x47 plays a motion. */
unsigned char func_08AA99D4(ScreenAB7CFC *self)
{
    return (self->work->handles[0x47]->field_20 & 0x20) != 0;
}
