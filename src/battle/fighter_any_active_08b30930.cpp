extern "C" {

struct ActionSlot {
    int field_00;
    unsigned char active;   /* 0x04 */
    unsigned char ready;    /* 0x05 */
    char pad06[2];
};

struct FighterSlots {
    char pad0000[0x1028];
    ActionSlot slots[8];    /* 0x1028 */
    short count;            /* 0x1068 */
};

int func_08B30930(FighterSlots *fighter)
{
    for (int i = 0; i < fighter->count; i++) {
        if (fighter->slots[i].active != 0) {
            return 1;
        }
    }
    return 0;
}

int func_08B3097C(FighterSlots *fighter)
{
    for (int i = 0; i < fighter->count; i++) {
        if (fighter->slots[i].ready != 0) {
            return 1;
        }
    }
    return 0;
}

}
