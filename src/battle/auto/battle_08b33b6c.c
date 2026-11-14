#include "battle/auto_region5.h"

/* Fighter: with a negative arg2 runs func_08B33B08; otherwise hands the
 * current entry of field_30C to func_088B7A88. */
void func_08B33B6C(BattleFighter5 *fighter, int arg1, int arg2)
{
    if (arg2 < 0) {
        func_08B33B08(fighter);
    } else {
        func_088B7A88(fighter->field_30C->entries[fighter->field_30C->field_773], arg2, arg1);
    }
}
