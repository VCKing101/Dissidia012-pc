#include "menu/auto_region3.h"

/* Btn77168: true while node field_00 plays a motion. */
unsigned char func_08A772AC(Btn77168 *self)
{
    unsigned char playing;

    playing = 0;
    if (self->field_00.model != 0) {
        playing = (self->field_00.model->field_20 & 0x20) != 0;
    }
    return playing;
}
