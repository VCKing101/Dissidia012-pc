#include "battle/auto_region3.h"

/* Frees the three handles in the state data and clears flag 0x200000. */
void func_08A2C8DC(void *self, BattleFighter *fighter)
{
    FighterWork *work;

    work = fighter->work;
    if (work->field_40 != 0) {
        func_088A6AA4(work->field_40);
        work->field_40 = 0;
    }
    if (work->field_44 != 0) {
        func_088A6AA4(work->field_44);
        work->field_44 = 0;
    }
    if (work->field_48 != 0) {
        func_088A6AA4(work->field_48);
        work->field_48 = 0;
    }
    fighter->flags &= ~0x200000;
}
