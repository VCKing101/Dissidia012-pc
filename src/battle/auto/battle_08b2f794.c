#include "battle/auto_region5.h"

/* Fighter: field_1077, adjusted by field_39C's field_06 when
 * func_08B3459C holds. */
unsigned int func_08B2F794(BattleFighter5 *fighter)
{
    unsigned int value;

    value = fighter->field_1077;
    if (func_08B3459C((BattleFighter *)fighter) != 0) {
        value = (value - fighter->field_39C->field_06) + 10;
    }
    return value;
}
