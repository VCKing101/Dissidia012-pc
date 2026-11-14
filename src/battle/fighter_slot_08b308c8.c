typedef struct ActionSlot {
    int field_00;
    unsigned char active;   /* 0x04 */
    unsigned char ready;    /* 0x05 */
    char pad06[2];
} ActionSlot;

typedef struct FighterSlots {
    char pad0000[0x1028];
    ActionSlot slots[8];    /* 0x1028 */
    short count;            /* 0x1068 */
} FighterSlots;

ActionSlot *func_08B308C8(FighterSlots *fighter, int index)
{
    ActionSlot *slots = fighter->slots;
    return &slots[index];
}
