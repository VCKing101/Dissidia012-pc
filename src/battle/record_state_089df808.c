#include "battle/agent1_types.h"

/* Record state helpers: refresh, counter, value and model alpha. */
extern void func_089E0110(BattleRecord54 *);
extern void func_089E02DC(BattleRecord54 *);
extern void func_089E033C(BattleRecord54 *);
extern void func_088DA0A8(void *, int, void *, void *);

int func_089DF808(BattleRecord54 *record)
{
    if (!record->field_00) return -1;
    func_089E0110(record);
    func_089E02DC(record);
    func_089E033C(record);
    return 0;
}

int func_089DF854(BattleRecord54 *record, int delta)
{
    if (!record->field_00) return -1;
    record->field_04 += delta;
    return 0;
}

int func_089DF880(BattleRecord54 *record)
{
    if (!record->field_00) return 0;
    return record->field_08;
}

float func_089DF8A0(BattleRecord54 *record)
{
    if (!record->field_00) return 0.0f;
    if (!record->field_30) return 1.0f;
    {
        float matrix[16];
        unsigned char color[4];
        short kind = record->field_30->field_0C;
        if (kind == 0) {
            func_088DA0A8(record->field_30, 0, matrix, color);
        } else if (kind == 1) {
            func_088DA0A8(record->field_30, 0, matrix, color);
        } else {
            func_088DA0A8(record->field_30, 0, matrix, color);
        }
        return color[3] / 255.0f;
    }
}
