#include "menu/auto_region3.h"

/* Record16DC: the value of kind 0 at field_00. */
int func_08AB1748(Record16DC *self)
{
    int *value;

    value = func_08AB0F80(self, 0, self->field_00);
    return *value;
}
