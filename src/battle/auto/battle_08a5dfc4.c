#include "battle/auto_region3.h"

/* Calls func_08837A38, hands the vector to func_08A5BF84 and enters state 1. */
void func_08A5DFC4(void *self, BattleFighter *fighter, const BattleVec4 *vector)
{
    func_08837A38(fighter);
    func_08A5BF84(fighter, vector);
    func_08834C10(fighter, 1, 0);
}
