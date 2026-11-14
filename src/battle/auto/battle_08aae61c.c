#include "battle/auto_region4.h"

/* Calls func_08AABE2C with `arg` on the base part of the battle's
 * Part80208 (D_08A80218). */
void func_08AAE61C(void *unused, int arg)
{
    Part80208 *instance;

    instance = func_08A80200(D_08A80218);
    func_08AABE2C((PartADF68 *)instance->base, arg);
}
