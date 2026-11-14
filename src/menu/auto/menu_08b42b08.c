#include "menu/auto_region4.h"

/* Heap42AA0: destroys its heap and releases its object. */
void func_08B42B08(Heap42AA0 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->object);
    self->object = 0;
}
