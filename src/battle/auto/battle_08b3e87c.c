#include "battle/auto_region5.h"

/* Is `other`'s field_FC set? */
unsigned char func_08B3E87C(void *self, Node3E87C *other)
{
    return other->field_FC != 0;
}
