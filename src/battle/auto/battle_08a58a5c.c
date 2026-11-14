#include "battle/auto_region3.h"

/* Hands the vector to func_08A5BF84, then sets field_F4 of field_314 to 2. */
void func_08A58A5C(BattleFighter *fighter, const BattleVec4 *vector)
{
    func_08A5BF84(fighter, vector);
    fighter->field_314->field_F4 = 2;
}
