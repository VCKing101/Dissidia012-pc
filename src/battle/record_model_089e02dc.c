/* Drops the record's attached model once it reports the 0x20 state flag. */

#include "battle/agent1_types.h"
extern void func_088DA060(void *);

int func_089E02DC(BattleRecord54 *record)
{
    if (record->field_28) {
        if ((unsigned char)((record->field_28->field_20 & 0x20) ? 1 : 0) == 1) {
            if (record->field_28) {
                func_088DA060(record->field_28);
                record->field_28 = 0;
            }
        }
    }
    return 0;
}

