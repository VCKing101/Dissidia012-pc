#include "battle/auto_region4.h"

/* Cross product of a and b on the X/Z plane (a.z * b.x - a.x * b.z). */
float func_08A86264(const BattleVec3 *a, const BattleVec3 *b)
{
    return a->z * b->x - a->x * b->z;
}
