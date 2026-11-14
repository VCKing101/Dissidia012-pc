#include "battle/auto_region5.h"

/* Fighter: is field_6E0 1? */
unsigned char func_08B333C8(BattleFighter5 *fighter)
{
    return fighter->field_6E0 == 1;
}
