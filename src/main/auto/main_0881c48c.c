#include "main/auto_region1.h"

/* value while active, otherwise 0. */
unsigned int func_0881C48C(ActiveWord *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->value;
    }
}
