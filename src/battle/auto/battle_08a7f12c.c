#include "battle/auto_region3.h"

/* field_9C as an integer while active, otherwise 0. */
int func_08A7F12C(Part7FF38 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return (int)self->field_9C;
    }
}
