#include "menu/auto_region4.h"

int func_08B2F98C(Obj2F8F4 *self)
{
    int result;
    MenuIntCallback callback;

    callback = self->work->field_9D0;
    result = 0;
    if (callback != 0) {
        result = callback();
    }
    return result;
}
