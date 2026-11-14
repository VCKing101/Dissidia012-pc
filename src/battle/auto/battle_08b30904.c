#include "battle/auto_region5.h"

/* Fighter: clears field_1068 and the eight slots at field_1028. */
void func_08B30904(BattleFighter5 *fighter)
{
    int i;

    fighter->field_1068 = 0;
    for (i = 0; i < 8; i++) {
        fighter->field_1028[i].field_00 = 0;
        fighter->field_1028[i].field_04 = 0;
        fighter->field_1028[i].field_05 = 0;
    }
}
