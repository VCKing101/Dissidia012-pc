#include "battle/agent1_types.h"

/* Destructor, reset and release of one 0x54-byte battle record. */
extern unsigned char D_089E05F0[];
extern void func_088DA060(void *);
extern void func_08992F04(void *);
void func_089DF598(BattleRecord54 *record);
void func_089DF628(BattleRecord54 *record);

void func_089DF540(BattleRecord54 *record, int flags)
{
    if (record) {
        record->vtable = D_089E05F0;
        func_089DF628(record);
        if (flags & 1) func_08992F04(record);
    }
}

void func_089DF598(BattleRecord54 *record)
{
    record->field_00 = 0;
    record->field_04 = 0;
    record->field_08 = 0;
    record->field_0C = 0;
    record->field_10 = -1;
    record->field_14 = -1;
    record->field_18 = -1;
    {
        int i;
        for (i = 0; i < 7; i++) {
            *((unsigned char *)record + 0x1C + i) = 0;
        }
    }
    {
        int i;
        for (i = 0; i < 3; i++) {
            *((unsigned char *)record + 0x23 + i) = 0;
        }
    }
    record->field_28 = 0;
    record->field_2C = 0;
    record->field_30 = 0;
    {
        int i;
        for (i = 0; i < 4; i++) {
            record->field_34[i] = 0;
        }
    }
    record->field_44 = -1;
    record->field_48 = 0;
    record->field_4C = 0;
}

void func_089DF628(BattleRecord54 *record)
{
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (record->field_34[i]) {
                func_088DA060(record->field_34[i]);
                record->field_34[i] = 0;
            }
        }
    }
    if (record->field_30) {
        func_088DA060(record->field_30);
        record->field_30 = 0;
    }
    if (record->field_2C) {
        func_088DA060(record->field_2C);
        record->field_2C = 0;
    }
    if (record->field_28) {
        func_088DA060(record->field_28);
        record->field_28 = 0;
    }
    func_089DF598(record);
}
