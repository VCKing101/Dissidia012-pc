extern "C" {

struct FreeFlags {
    unsigned char bytes[4];
};

struct FighterFree {
    char pad000[0x5B0];
    FreeFlags flags;        /* 0x5B0 */
};

int func_08837B9C(FighterFree *fighter);

struct FighterFreeOps {
    static unsigned char locked(FreeFlags *flags)
    {
        unsigned int bit = ((unsigned char *)flags)[3] & 1;
        return bit;
    }
    static unsigned char held(FreeFlags *flags)
    {
        return (((unsigned char *)flags)[3] & 2) != 0;
    }
    static unsigned char busy(FreeFlags *flags)
    {
        int result = 0;
        if (locked(flags) || held(flags)) {
            result = 1;
        }
        return result;
    }
};

int func_08B2FB6C(FighterFree *fighter)
{
    if (FighterFreeOps::busy(&fighter->flags)) {
        return 0;
    }
    return func_08837B9C(fighter);
}

}
