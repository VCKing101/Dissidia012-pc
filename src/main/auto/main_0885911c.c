#include "main/auto_region2.h"

/* Creates handle index and sets it up. */
void func_0885911C(Part859060 *self, int index, unsigned int *arg1, int arg2)
{
    int *handle;

    handle = func_0899B9F0(0);
    self->handles[index] = handle;
    func_0899BB68(handle, arg1, arg2, 0);
}
