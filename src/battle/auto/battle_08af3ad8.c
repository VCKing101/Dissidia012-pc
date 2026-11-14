#include "battle/auto_region4.h"

/* ObjFB520 deleting destructor: destroys the member at field_88, then the
 * base. */
void func_08AF3AD8(ObjFB520 *self, int flags)
{
    if (self) {
        self->vtable = D_08AFB520;
        func_0887B1F4(self->field_88, 2);
        func_088EC234(self, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
