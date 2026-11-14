#include "battle/auto_region3.h"

/* Enters state 10 and stores the scale (1 when not positive) in the state
 * data. */
void func_08A58098(BattleFighter *fighter, float scale)
{
    func_08834C10(fighter, 10, 0);
    if (scale <= 0.0f) {
        scale = 1.0f;
    }
    fighter->work->field_14 = scale;
}
