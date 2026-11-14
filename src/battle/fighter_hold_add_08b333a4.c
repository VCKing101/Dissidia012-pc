typedef struct FighterHold {
    char pad0000[0x10C8];
    short hold;             /* 0x10C8 */
    short holdExtra;        /* 0x10CA */
} FighterHold;

void func_08B333A4(FighterHold *fighter, int delta)
{
    fighter->holdExtra += delta;
}
