#include "battle/auto_region5.h"

/* Handle4C22C: gives the handle the matrix, if any. */
void func_08B4C494(Handle4C22C *self, const void *matrix)
{
    if (self->handle != 0) {
        func_088D9B84(self->handle, matrix);
    }
}
