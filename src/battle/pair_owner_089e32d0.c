#include "battle/agent1_types.h"

/* Reset and release of the 0x18 subobject of the battle setup root. */
extern void func_089E39CC(void *);

void func_089E32D0(BattlePairOwner *owner)
{
    int i;

    owner->field_00 = 0;
    owner->field_04 = 0;
    for (i = 0; i < 3; i++) {
        *((unsigned char *)owner + 0x1B8 + i) = 0;
    }
    owner->field_1BB = 0;
    owner->field_1BC = 0;
}

void func_089E3300(BattlePairOwner *owner)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
            func_089E39CC(&owner->field_08[i]);
        }
    }
    func_089E32D0(owner);
}
