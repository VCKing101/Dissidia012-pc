#include "battle/agent1_types.h"

/* Constructor, destructor, reset and release of the 0x2C-byte effect slot. */
extern unsigned char D_089E51E0[];
extern void func_088DA060(void *);
extern void func_08992F04(void *);
void func_089E4C20(BattleEffectSlot *slot);
void func_089E4C50(BattleEffectSlot *slot);

BattleEffectSlot *func_089E4B90(BattleEffectSlot *slot)
{
    slot->vtable = D_089E51E0;
    func_089E4C20(slot);
    return slot;
}

void func_089E4BC8(BattleEffectSlot *slot, int flags)
{
    if (slot) {
        slot->vtable = D_089E51E0;
        func_089E4C50(slot);
        if (flags & 1) func_08992F04(slot);
    }
}

void func_089E4C20(BattleEffectSlot *slot)
{
    slot->field_00 = 0;
    slot->field_04 = -1;
    slot->field_08 = 0;
    slot->field_0C = 0;
    slot->field_10 = 0;
    slot->field_14 = -1;
    slot->field_18 = 0;
    slot->field_1C = 0;
    slot->field_20 = 0;
    slot->field_24 = 0;
}

void func_089E4C50(BattleEffectSlot *slot)
{
    if (slot->field_10) {
        func_088DA060(slot->field_10);
        slot->field_10 = 0;
    }
    if (slot->field_0C) {
        func_088DA060(slot->field_0C);
        slot->field_0C = 0;
    }
    if (slot->field_08) {
        func_088DA060(slot->field_08);
        slot->field_08 = 0;
    }
    func_089E4C20(slot);
}
