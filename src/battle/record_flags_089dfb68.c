#include "battle/agent1_types.h"

/* Record flag toggles. */
extern int func_089E05AC(BattleRecord54 *);
extern void func_089DFF50(BattleRecord54 *);
extern void func_088DA060(void *);

int func_089DFB68(BattleRecord54 *record, unsigned int index)
{
    int before;

    if (!record->field_00) return -1;
    if (index >= 7) return -2;
    before = func_089E05AC(record);
    record->field_1C[index] = 0;
    if (before != func_089E05AC(record)) {
        func_089DFF50(record);
    }
    return 0;
}

int func_089DFBF8(BattleRecord54 *record, unsigned int index)
{
    int before;

    if (!record->field_00) return -1;
    if (index >= 7) return -2;
    before = func_089E05AC(record);
    record->field_1C[index] = 1;
    if (before != func_089E05AC(record)) {
        func_089DFF50(record);
        if (record->field_28) {
            func_088DA060(record->field_28);
            record->field_28 = 0;
        }
    }
    return 0;
}
