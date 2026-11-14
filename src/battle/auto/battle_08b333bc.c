#include "battle/auto_region5.h"

/* Fighter: is field_6E0 0? */
unsigned char func_08B333BC(BattleFighter5 *fighter)
{
    return fighter->field_6E0 == 0;
}
