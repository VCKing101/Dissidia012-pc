#include "menu/auto_region4.h"

/* Panel26C88: the prompt is open (flag 4) and answered 1. */
int func_08B271C8(Panel26C88 *self)
{
    int result;

    result = 0;
    if (((self->work->flags & 4) != 0) && (func_088D4FBC(&self->work->prompt) == 1)) {
        result = 1;
    }
    return result;
}
