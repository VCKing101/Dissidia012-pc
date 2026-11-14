/* Destructor of one 0x1C-byte slot of the 0x1C6C slot owner. */
typedef struct BattleSlotTail {
    unsigned char padding_00[0x18];
    void *vtable;
} BattleSlotTail;

extern unsigned char D_089E1BE8[];
extern void func_089E1268(void *);
extern void func_08992F04(void *);

void func_089E11F0(BattleSlotTail *slot, int flags)
{
    if (slot) {
        slot->vtable = D_089E1BE8;
        func_089E1268(slot);
        if (flags & 1) func_08992F04(slot);
    }
}
