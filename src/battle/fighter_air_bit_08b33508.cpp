extern "C" {

struct FighterAirBitFlags {
    unsigned char bytes[8];
};

struct FighterAirBit {
    char pad000[0x190];
    FighterAirBitFlags state;       /* 0x190 */
    char pad198[0x418];
    FighterAirBitFlags flags;       /* 0x5B0 */
};

struct FighterAirBitOps {
    static unsigned char locked(FighterAirBitFlags *flags)
    {
        return (((unsigned char *)flags)[4] & 2) != 0;
    }
    static int test(FighterAirBitFlags *state)
    {
        return (((unsigned char *)state)[4] & 0x80) != 0;
    }
};

int func_08B33508(FighterAirBit *fighter)
{
    if (FighterAirBitOps::locked(&fighter->flags)) {
        return 0;
    }
    return FighterAirBitOps::test(&fighter->state);
}

}
