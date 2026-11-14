#include "menu/auto_region3.h"

/* Record16DC: the value of kind 3 at field_03. */
int func_08AB144C(Record16DC *self)
{
    int *value;

    value = func_08AB0F80(self, 3, self->field_03);
    return *value;
}
