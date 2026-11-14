#include "battle/auto_region4.h"

/* PartE7B50: once field_2A8 and the fighter are idle, calls
 * func_08889F70/func_08889FB8 for the fighter's field_10CC and moves to
 * phase 4. */
void func_08AE1040(PartE7B50 *self)
{
    int busy;

    busy = func_08ADFDDC(self->field_2A8);
    if (busy == 0 && func_08838074(self->fighter) == 0) {
        func_08889F70(self->fighter->field_10CC, 0);
        func_08889FB8(self->fighter->field_10CC, 0, 1);
        func_08AE1270(self, 4);
    }
}
