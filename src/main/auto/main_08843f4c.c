#include "main/auto_region2.h"

/* Counts a use of the entry; the first one sets it up (func_08843FBC). */
void func_08843F4C(Entry9D4960 *self)
{
    if (self->field_E0 == 0) {
        func_08843FBC(self);
    }
    self->field_E0 = self->field_E0 + 1;
}
