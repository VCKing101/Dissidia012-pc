#include "battle/auto_region3.h"

/* Hands arg to func_08A97404 on what func_08AB3264 returns for field_20. */
void func_08A640CC(Part64E48 *self, int arg)
{
    void *member;

    member = func_08AB3264(self->field_20);
    func_08A97404(member, arg);
}
