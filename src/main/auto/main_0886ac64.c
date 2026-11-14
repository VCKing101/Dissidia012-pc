#include "main/auto_region2.h"

/* Calls callback_12C with its argument and the model handle. */
void func_0886AC64(ModelObj *self)
{
    if (self->callback_12C != 0) {
        self->callback_12C(self->arg_130, self->data->handle);
    }
}
