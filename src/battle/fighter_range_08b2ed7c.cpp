extern "C" {

struct RangeFlags {
    unsigned char bytes[12];
};

struct FighterRange {
    char pad000[0x190];
    RangeFlags flags;       /* 0x190 */
};

void func_08B34E98(FighterRange *fighter, int reason);

struct FighterRangeOps {
    static unsigned char tracked(RangeFlags *flags)
    {
        unsigned int bit = ((unsigned char *)flags)[9] & 1;
        return bit;
    }
};

void func_08B2ED7C(FighterRange *fighter, int value)
{
    if (FighterRangeOps::tracked(&fighter->flags)) {
        if (value < 500 || value >= 0x31F) {
            func_08B34E98(fighter, 5);
        }
    }
}

}
