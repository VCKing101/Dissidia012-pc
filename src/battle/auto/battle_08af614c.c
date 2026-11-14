#include "battle/auto_region5.h"

/* PartF6008: stores an integer as the float field_F68. The earlier header
 * declares this with a void * parameter. */
void func_08AF614C(void *object, int value)
{
    PartF6008 *self = object;

    self->field_F68 = (float)value;
}
