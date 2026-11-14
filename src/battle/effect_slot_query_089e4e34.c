#include "battle/agent1_types.h"

/* Effect slot queries against the bound unit. */
typedef struct BattleShortHead {
    short field_00;
} BattleShortHead;

typedef struct BattleUnitPhase {
    BattleShortHead *field_00;
    signed char field_04;
} BattleUnitPhase;

typedef struct BattleUnitView {
    unsigned char padding_000[0xD5C];
    BattleUnitPhase field_D5C;
} BattleUnitView;

extern BattleUnitView *func_089E95B0(int);

int func_089E4E34(BattleEffectSlot *slot)
{
    BattleUnitView *unit = func_089E95B0(slot->field_14);
    BattleUnitPhase *phase;

    if (!unit) return -1;
    phase = &unit->field_D5C;
    if (phase->field_00 != 0) {
        BattleShortHead *head = phase->field_00;
        signed char kind = phase->field_04;
        if (head->field_00 == 0 && kind == 1) return 1;
    }
    return 0;
}

int func_089E4EA4(BattleEffectSlot *slot, unsigned int index)
{
    int values[2];

    values[0] = slot->field_18;
    values[1] = slot->field_1C;
    if (index >= 2) return 0;
    return values[index];
}
