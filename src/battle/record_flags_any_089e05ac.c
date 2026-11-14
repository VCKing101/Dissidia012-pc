#include "battle/agent1_types.h"

/* Reports whether any of the record's seven flags is set. */
int func_089E05AC(BattleRecord54 *record)
{
    int result = 0;
    {
        int i;
        for (i = 0; i < 7; i++) {
            unsigned char *flags = record->field_1C;
            if (flags[i] == 1) {
                result = 1;
                break;
            }
        }
    }
    return result;
}
