#include "menu/agent29_types.h"

/* Screen0B6A0: marks (bit 0 of the per-slot word at +0x2E98) every party
   slot that is set up (flag 1 or 2), and every slot after the first one
   with flag 8. */
struct PartySlot29 {
    char pad_00[0x3];
    unsigned char flags;
    char pad_04[0x10];

    static bool Has(PartySlot29 *slot, int mask) { return (slot->flags & mask) != 0; }
};

struct Party29 {
    PartySlot29 slots[5];
    int count; /* +0x64 */
};

struct PartyData29 {
    char pad_00[0x8];
    Party29 party;
};

extern "C" PartyData29 *func_088F29C4(void);

extern "C" void func_089FC4C0(Screen0B6A0 *self)
{
    PartyData29 *data;
    Party29 *party;
    int i;
    bool after;

    data = func_088F29C4();
    party = &data->party;
    after = false;
    for (i = 0; i < party->count; i++) {
        if (PartySlot29::Has(&data->party.slots[i], 8)) {
            after = true;
        }
        if (PartySlot29::Has(&party->slots[i], 2) || PartySlot29::Has(&party->slots[i], 1) || after) {
            (&WORK_INT(self->work, 0x2E98))[i] |= 1;
        }
    }
}
