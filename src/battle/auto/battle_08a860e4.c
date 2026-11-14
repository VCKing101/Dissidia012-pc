#include "battle/auto_region4.h"

/* Cross product of (a - origin) and (b - origin) on the X/Z plane. */
float func_08A860E4(const BattleVec3 *a, const BattleVec3 *b, const BattleVec3 *origin)
{
    return (a->x - origin->x) * (b->z - origin->z) -
           (a->z - origin->z) * (b->x - origin->x);
}
