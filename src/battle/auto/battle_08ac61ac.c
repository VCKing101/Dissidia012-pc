#include "battle/auto_region4.h"

/* PartC6098: creates handle `index` and starts it with the two arguments. */
void func_08AC61AC(PartC6098 *self, int index, int arg2, int arg3)
{
    void *handle;

    handle = func_0899B9F0(0);
    self->handles[index] = handle;
    func_0899BB68(handle, arg2, arg3, 0);
}
