#include "battle/auto_region5.h"

/* Handle41620: loads the handle from `source`. */
void func_08B41638(Handle41620 *self, void *source)
{
    self->handle = func_088DAA68(source);
    self->phase = 0;
    self->field_08 = 0;
}
