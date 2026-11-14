#include "main/auto_region2.h"

/* Calls callback_134 with its argument and the model handle. */
void func_0886AC98(ModelObj *self)
{
    if (self->callback_134 != 0) {
        self->callback_134(self->arg_138, self->data->handle);
    }
}
