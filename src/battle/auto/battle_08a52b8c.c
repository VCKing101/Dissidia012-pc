#include "battle/auto_region3.h"

/* Clears field_538 unless the state data says otherwise, then frees its
 * handle. */
void func_08A52B8C(void *self, BattleFighter *fighter)
{
    FighterWork *work;

    work = fighter->work;
    if (work->field_2E == 0) {
        fighter->field_538 = 0;
    }
    if (work->field_30 != 0) {
        func_088A6AA4(work->field_30);
        work->field_30 = 0;
    }
}
