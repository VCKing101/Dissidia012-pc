#include "main/auto_region2.h"

/* Creates the handle and sets it up. */
void func_08854254(Handle *self, unsigned int *arg1, int arg2)
{
    int *handle;

    handle = func_0899B9F0(0);
    self->handle = handle;
    func_0899BB68(handle, arg1, arg2, 0);
}
