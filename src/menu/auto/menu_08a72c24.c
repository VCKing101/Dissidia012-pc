#include "menu/auto_region3.h"

/* Stage72ABC: runs func_08A72C58; true while node field_08 plays a motion. */
unsigned char func_08A72C24(Stage72ABC *self)
{
    func_08A72C58(self);
    return (self->field_08.model->field_20 & 0x20) != 0;
}
