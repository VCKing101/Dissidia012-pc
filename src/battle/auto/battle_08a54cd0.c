#include "battle/auto_region3.h"

/* Enters state 8; copies field_1077 to field_1074 when func_08B3459C reports
 * 0. */
void func_08A54CD0(BattleFighter *fighter)
{
    func_08834C10(fighter, 8, 0);
    if (func_08B3459C(fighter) == 0) {
        fighter->field_1074 = fighter->field_1077;
    }
}
