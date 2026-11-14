#include "menu/auto_region3.h"

/* View8E3B4: the record number of the current index. */
int func_08A8EF14(View8E3B4 *self)
{
    return self->work->field_308[self->work->index];
}
