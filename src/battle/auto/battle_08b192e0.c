#include "battle/auto_region5.h"

/* Part19300: field_18 while active, else 0. */
unsigned int func_08B192E0(Part19300 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_18;
    }
}
