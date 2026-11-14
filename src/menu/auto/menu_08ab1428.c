#include "menu/auto_region3.h"

/* Record16DC: the value of kind 2 at field_02. */
int func_08AB1428(Record16DC *self)
{
    int *value;

    value = func_08AB0F80(self, 2, self->field_02);
    return *value;
}
