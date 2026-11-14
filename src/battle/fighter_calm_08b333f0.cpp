extern "C" {

struct CalmFlags {
    unsigned char bytes[4];
};

struct FighterCalm {
    char pad0000[0x5B0];
    CalmFlags flags;        /* 0x5B0 */
    char pad05B4[0xAC0];
    unsigned char field_1074;
    unsigned char field_1075;
    unsigned char field_1076;
    char pad1077[0x234];
    unsigned char field_12AB;   /* 0x12AB */
};

struct FighterCalmOps {
    static void clear(CalmFlags *flags)
    {
        ((unsigned char *)flags)[3] &= ~4;
    }
};

void func_08B333F0(FighterCalm *fighter)
{
    fighter->field_1074 = 0;
    fighter->field_1075 = 0;
    fighter->field_1076 = 0;
    FighterCalmOps::clear(&fighter->flags);
    fighter->field_12AB = 0;
}

}
