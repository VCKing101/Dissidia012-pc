#include "battle/auto_region3.h"

/* Frees the handle in the state data and clears field_538. */
void func_08A507B8(void *self, BattleFighter *fighter)
{
    FighterWork *work;

    work = fighter->work;
    if (work->field_20 != 0) {
        func_088A6AA4(work->field_20);
        work->field_20 = 0;
    }
    fighter->field_538 = 0;
}
