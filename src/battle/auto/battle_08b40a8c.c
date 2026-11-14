#include "battle/auto_region5.h"

/* Does `other`'s field_F8 have entry 0x2F (func_088BF4CC)? */
unsigned char func_08B40A8C(void *self, Part40A50 *other)
{
    void *object;
    int found;

    object = func_08835778(other->field_F8);
    found = func_088BF4CC(object, 0x2F);
    return found != 0;
}
