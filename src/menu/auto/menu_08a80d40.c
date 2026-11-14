#include "menu/auto_region3.h"

/* Deleting destructor (func_08A7EB48 releases it). */
void func_08A80D40(void *object, unsigned int flags)
{
    if ((object != 0) && (func_08A7EB48(object), (flags & 1) != 0)) {
        func_08992F04(object);
    }
}
