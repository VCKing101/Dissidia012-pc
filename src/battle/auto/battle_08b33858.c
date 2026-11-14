#include "battle/auto_region5.h"

/* Fighter: runs func_088316B8 between func_08AC5958 and func_08AC5A64 on
 * field_BA0. */
void func_08B33858(BattleFighter5 *fighter)
{
    func_08AC5958(fighter->field_BA0);
    func_088316B8(fighter);
    func_08AC5A64(fighter->field_BA0);
}
