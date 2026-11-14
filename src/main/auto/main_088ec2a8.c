#include "main/auto_region4.h"

/* Handles8EC278: reads its three handles from source. */
void func_088EC2A8(Handles8EC278 *self, void *source)
{
    void *handle;

    handle = func_088DAA68(source);
    self->handles[0] = handle;
    handle = func_088DAA68(source);
    self->handles[1] = handle;
    handle = func_088DAA68(source);
    self->handles[2] = handle;
}
