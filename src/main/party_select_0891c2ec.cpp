extern "C" {
typedef struct PartySlot {
    int id;
    char data[0x20];
} PartySlot;

typedef struct PartyPick {
    char pad_000[0x2A24];
    PartySlot *slot;            /* 0x2A24 */
    void (*done)(void);         /* 0x2A28 */
} PartyPick;

extern void func_0891C790(void (*done)(void));

bool func_0891C2EC(PartyPick *party, PartySlot *slot, void (*done)(void))
{
    if (slot->id != -1) {
        party->slot = slot;
        party->done = done;
        func_0891C790(done);
        return true;
    }
    return false;
}
}
