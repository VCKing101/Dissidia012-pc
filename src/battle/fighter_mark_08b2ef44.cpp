extern "C" {

struct MarkFlags {
    unsigned char bytes[4];
};

struct FighterMark {
    char pad000[0x5AC];
    void *marker;           /* 0x5AC */
    MarkFlags flags;        /* 0x5B0 */
};

void func_08B3B310(void *marker, int on);

struct FighterMarkOps {
    static void set(MarkFlags *flags)
    {
        ((unsigned char *)flags)[0] |= 4;
    }
};

void func_08B2EF44(FighterMark *fighter)
{
    FighterMarkOps::set(&fighter->flags);
    func_08B3B310(fighter->marker, 1);
}

}
