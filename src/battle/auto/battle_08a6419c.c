#include "battle/auto_region3.h"

/* Hands arg to func_08A978F4 on what func_08AB3264 returns for field_20,
 * then calls func_08AA2E04 on field_13A0. */
int func_08A6419C(Part64E48 *self, int arg)
{
    void *member;

    member = func_08AB3264(self->field_20);
    func_08A978F4(member, arg);
    func_08AA2E04(self->field_13A0);
    return 0;
}
