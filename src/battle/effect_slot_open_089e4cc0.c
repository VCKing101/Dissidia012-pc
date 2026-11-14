#include "battle/agent1_types.h"

/* Opens an effect slot for a side; a side that is not yet set up gets its
 * effect model from the source, picked by the fighter's body type. */
typedef struct SlotFighter {
    unsigned char pad_000[0x5A4];
    unsigned char *body;
} SlotFighter;

extern void func_089E4C50(BattleEffectSlot *slot);
extern unsigned int func_089E95E0(unsigned int side);
extern int func_089E98F0(unsigned int side);
extern SlotFighter *func_089E95B0(unsigned int side);
extern void *func_088DAA40(void *source, int name, int flags);
extern int D_089E5208[2];
extern int D_089E5210[2];

int func_089E4CC0(BattleEffectSlot *slot, unsigned int side, int a, int b, void *source, int c, int d)
{
    unsigned int index;

    func_089E4C50(slot);
    if (side >= 2 || source == 0 || d == 0) {
        return -1;
    }
    index = func_089E95E0(side);
    if (index >= 2) {
        return -2;
    }
    if (func_089E98F0(side) == 0) {
        SlotFighter *fighter = func_089E95B0(side);
        if (fighter != 0 && *fighter->body == 0x20) {
            slot->field_0C = func_088DAA40(source, D_089E5210[index], 0);
        } else {
            slot->field_0C = func_088DAA40(source, D_089E5208[index], 0);
        }
        if (slot->field_0C == 0) {
            return -3;
        }
    }
    slot->field_14 = side;
    slot->field_18 = a;
    slot->field_1C = b;
    slot->field_20 = c;
    slot->field_24 = d;
    slot->field_00 = 1;
    return 0;
}
