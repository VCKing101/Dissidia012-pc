extern "C" {

struct FighterDashBitFlags {
    unsigned char bytes[8];
};

struct FighterDashBit {
    char pad000[0x190];
    FighterDashBitFlags state;       /* 0x190 */
    char pad198[0x418];
    FighterDashBitFlags flags;       /* 0x5B0 */
};

struct FighterDashBitOps {
    static unsigned char locked(FighterDashBitFlags *flags)
    {
        return (((unsigned char *)flags)[4] & 2) != 0;
    }
    static int test(FighterDashBitFlags *state)
    {
        return (((unsigned char *)state)[5] & 2) != 0;
    }
};

int func_08B33544(FighterDashBit *fighter)
{
    if (FighterDashBitOps::locked(&fighter->flags)) {
        return 0;
    }
    return FighterDashBitOps::test(&fighter->state);
}

}
