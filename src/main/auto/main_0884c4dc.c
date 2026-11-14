#include "main/auto_region2.h"

/* Creates the two handles of slot index and sets them up. */
void func_0884C4DC(Obj84C4DC *self, int index, unsigned int *arg1, int arg2, unsigned int *arg3, int arg4)
{
    int *handle;

    handle = func_0899B9F0(0);
    self->slots[index].field_00 = handle;
    handle = func_0899B9F0(0);
    self->slots[index].field_04 = handle;
    func_0899BB68(self->slots[index].field_00, arg1, arg2, 0);
    func_0899BB68(self->slots[index].field_04, arg3, arg4, 0);
}
