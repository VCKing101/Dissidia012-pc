#include "battle/auto_region5.h"

/* Handle41620 constructor: clears it. */
Handle41620 *func_08B41620(Handle41620 *self)
{
    self->handle = 0;
    self->phase = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    return self;
}
