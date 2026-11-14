#include "battle/auto_region5.h"

/* Handle4C22C: loads the handle from `source` and keeps two words. */
void func_08B4C264(Handle4C22C *self, void *source, int arg1, int arg2)
{
    self->handle = func_088DAA40(source, 7, 0);
    self->field_04 = arg1;
    self->field_08 = arg2;
}
