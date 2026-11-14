#include "battle/auto_region3.h"

/* field_24 while active, otherwise 0. */
unsigned char func_08A7EEF4(Part7FF38 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_24;
    }
}
