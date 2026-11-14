#include "menu/auto_region1.h"

/* Screen0B6A0: address of entry index of field_64. */
Pair8 *func_089F1CE0(Screen0B6A0 *self, short index)
{
    return &self->work->field_64[index];
}
