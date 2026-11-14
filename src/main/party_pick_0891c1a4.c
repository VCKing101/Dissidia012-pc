typedef struct PartySlot {
    int id;
    char data[0x20];
} PartySlot;

typedef struct Party {
    char pad_000[0x2728];
    PartySlot slots[8];         /* 0x2728 */
} Party;

extern int func_0891C2EC(Party *party, PartySlot *slot, void (*done)(void));
extern void func_0891C710(void);

int func_0891C1A4(Party *party, int index)
{
    if (index >= 0) {
        return func_0891C2EC(party, &party->slots[index], func_0891C710);
    }
    return 0;
}
