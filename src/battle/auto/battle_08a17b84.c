#include "battle/auto_region3.h"

/* Sets field_04 to -1 once field_50 (a Part174B8) has finished. */
int func_08A17B84(Part17C90 *self)
{
    if (func_08A17264(&self->field_50) == 1) {
        self->field_04 = -1;
    }
    return 0;
}
