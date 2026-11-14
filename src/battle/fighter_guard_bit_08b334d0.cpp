extern "C" {

struct FighterGuardBitFlags {
    unsigned char bytes[8];
};

struct FighterGuardBit {
    char pad000[0x190];
    FighterGuardBitFlags state;       /* 0x190 */
    char pad198[0x418];
    FighterGuardBitFlags flags;       /* 0x5B0 */
};

struct FighterGuardBitOps {
    static unsigned char locked(FighterGuardBitFlags *flags)
    {
        return (((unsigned char *)flags)[4] & 2) != 0;
    }
    static unsigned char test(FighterGuardBitFlags *state)
    {
        return ((unsigned char *)state)[5] & 1;
    }
};

unsigned char func_08B334D0(FighterGuardBit *fighter)
{
    if (FighterGuardBitOps::locked(&fighter->flags)) {
        return 0;
    }
    return FighterGuardBitOps::test(&fighter->state);
}

}
