#include "menu/auto_region4.h"

void func_08B2D928(Obj2D604 *self)
{
    void *value;
    int i;

    for (i = 0; i < 5; i++) {
        value = self->work->field_564[i];
        if (value != 0) {
            func_08B2DF50(self, i, value);
        }
    }
}
