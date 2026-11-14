#include "menu/auto_region2.h"

/* Sends owner's field_210 as a short (kind 4) through func_089050F8. */
void func_08A70270(void *self, Obj70270 *owner)
{
    short value[2];

    value[0] = owner->field_210;
    func_089050F8(owner, 4, value, 2);
}
