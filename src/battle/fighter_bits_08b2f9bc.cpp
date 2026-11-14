extern "C" {

struct BitFlags {
    unsigned char bytes[4];
};

struct FighterBits {
    char pad000[0x5B0];
    BitFlags flags;         /* 0x5B0 */
};

struct FighterBitsOps {
    static int bit3(BitFlags *flags)
    {
        return (((unsigned char *)flags)[0] & 8) != 0;
    }
    static unsigned char bit0(BitFlags *flags)
    {
        return ((unsigned char *)flags)[0] & 1;
    }
};

int func_08B2F9BC(FighterBits *fighter)
{
    return FighterBitsOps::bit3(&fighter->flags);
}

unsigned char func_08B2F9D0(FighterBits *fighter)
{
    return FighterBitsOps::bit0(&fighter->flags);
}

}
