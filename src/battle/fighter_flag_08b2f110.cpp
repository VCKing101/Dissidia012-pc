extern "C" {

struct FlagBlock {
    unsigned char bytes[4];
};

struct FighterFlag {
    char pad000[0x5B0];
    FlagBlock flags;        /* 0x5B0 */
};

struct FighterFlagOps {
    static void set(FlagBlock *flags)
    {
        ((unsigned char *)flags)[2] |= 0x20;
    }
};

void func_08B2F110(FighterFlag *fighter)
{
    FighterFlagOps::set(&fighter->flags);
}

}
