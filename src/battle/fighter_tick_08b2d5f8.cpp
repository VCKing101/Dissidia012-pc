extern "C" {

struct FighterFlags {
    unsigned char bytes[8];
};

struct FighterTick {
    char pad0000[0x380];
    unsigned int state;         /* 0x380 */
    char pad0384[0x22C];
    FighterFlags flags;         /* 0x5B0 */
    char pad05B8[0x1CC];
    char motion[0x8EE];         /* 0x784 */
    unsigned short counter;     /* 0x1072 */
};

void func_08ACB208(void *motion);
void func_08B2E114(FighterTick *fighter);

struct FighterTickOps {
    static unsigned char airborne(FighterTick *fighter)
    {
        return (fighter->state & 0x200) != 0;
    }
    static unsigned char held(unsigned char *flags)
    {
        return (flags[1] & 0x10) != 0;
    }
};

void func_08B2D5F8(FighterTick *fighter)
{
    func_08ACB208(fighter->motion);
    if (FighterTickOps::airborne(fighter) && !FighterTickOps::held(fighter->flags.bytes)) {
        fighter->counter++;
    }
    func_08B2E114(fighter);
}

}
