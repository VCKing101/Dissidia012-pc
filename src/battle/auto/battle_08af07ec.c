#include "battle/auto_region4.h"

typedef struct EventF07EC {
    char pad_00[4];
    int field_04;
} EventF07EC;

/* Calls func_08842D18 on the fighter's field_52C when func_08B344E4
 * reports 1. */
void func_08AF07EC(void *self, BattleFighter4 **fighters, EventF07EC *event)
{
    int result;
    BattleFighter4 *fighter;

    fighter = *fighters;
    result = func_08B344E4(fighter);
    if (result == 1) {
        func_08842D18(fighter->field_52C, event->field_04);
    }
}
