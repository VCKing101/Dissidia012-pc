typedef struct FighterHold {
    char pad0000[0x10C8];
    short hold;             /* 0x10C8 */
    short holdExtra;        /* 0x10CA */
    char pad10CC[0x1D8];
    unsigned char holding;  /* 0x12A4 */
} FighterHold;

void func_08B33384(FighterHold *fighter, int hold)
{
    fighter->holding = 1;
    fighter->hold = hold;
}
