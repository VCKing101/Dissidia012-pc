#include "main/auto_region1.h"

/* Removes self's id from the bit sets of every listed actor that has a link. */
void func_0880826C(Actor *self)
{
    Actor *actor;

    actor = 0;
    while (actor = func_08841F64(actor), actor != 0) {
        if (actor->link != 0) {
            func_0880AE5C(&actor->link->bits, self);
        }
    }
}
