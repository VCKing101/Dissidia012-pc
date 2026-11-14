#include "battle/auto_region3.h"

/* Sets field_08 to 0xB once field_144 (a Part08388) has finished. */
int func_08A153C4(Unit15448 *self)
{
    if (func_08A0836C(&self->field_144) == 1) {
        self->field_08 = 0xB;
    }
    return 0;
}
