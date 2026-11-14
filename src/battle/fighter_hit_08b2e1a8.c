typedef struct HitSlot {
    int field_00;
    int field_04;
    float field_08;
    int field_0C;
} HitSlot;

typedef struct FighterHits {
    char pad000[0x5D8];
    HitSlot slots[16];      /* 0x5D8 */
    unsigned int mask;      /* 0x6D8 */
} FighterHits;

void func_08B2E1A8(FighterHits *fighter, int index, int a, int b, float c, int d)
{
    if (index >= 0 && index < 16) {
        fighter->mask |= 1 << index;
        fighter->slots[index].field_00 = a;
        fighter->slots[index].field_04 = b;
        fighter->slots[index].field_08 = c;
        fighter->slots[index].field_0C = d;
    }
}
