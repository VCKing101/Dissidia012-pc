#include "main/auto_region4.h"

/* Handles8EC4C4: reads its two handles from source. */
void func_088EC500(Handles8EC4C4 *self, void *source)
{
    void *handle;

    handle = func_088DAA68(source);
    self->handles[0] = handle;
    handle = func_088DAA68(source);
    self->handles[1] = handle;
}
