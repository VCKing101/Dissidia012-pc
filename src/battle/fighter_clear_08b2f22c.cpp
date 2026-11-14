extern "C" {

struct ClearFlags {
    unsigned char bytes[4];
};

struct FighterClear {
    char pad000[0x2FC];
    int field_2FC;          /* 0x2FC */
    char pad300[0x2B0];
    ClearFlags flags;       /* 0x5B0 */
};

struct FighterClearOps {
    static unsigned char pending(ClearFlags *flags)
    {
        unsigned int bit = ((unsigned char *)flags)[0] & 1;
        return bit;
    }
    static void clearPending(ClearFlags *flags)
    {
        ((unsigned char *)flags)[0] &= ~1;
    }
    static unsigned char kept(ClearFlags *flags)
    {
        return (((unsigned char *)flags)[2] & 0x80) != 0;
    }
};

void func_08B2F22C(FighterClear *fighter)
{
    if (FighterClearOps::pending(&fighter->flags)) {
        FighterClearOps::clearPending(&fighter->flags);
    }
    if (!FighterClearOps::kept(&fighter->flags)) {
        fighter->field_2FC = 0;
    }
}

}
