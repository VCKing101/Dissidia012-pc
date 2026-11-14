#include "battle/auto_region5.h"

/* Part1A0D8: field_08 while active, else 0. */
unsigned int func_08B1A610(Part1A0D8 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_08;
    }
}
