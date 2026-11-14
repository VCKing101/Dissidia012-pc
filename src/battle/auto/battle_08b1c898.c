#include "battle/auto_region5.h"

/* Part1C008: field_10 while active, else 0. */
unsigned int func_08B1C898(Part1C008 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_10;
    }
}
