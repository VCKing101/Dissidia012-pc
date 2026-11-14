#include "main/auto_region2.h"

/* Whether there is no model, or its handle is in state 1. */
unsigned char func_088864AC(Obj886208 *self)
{
    unsigned char result;

    result = 0;
    if (self->model == 0) {
        result = 1;
    } else if (self->handle != 0) {
        result = self->handle->state == 1;
    }
    return result;
}
