extern "C" {

struct AirFlags {
    unsigned int words[2];
};

struct FighterAir {
    char pad0000[0x380];
    unsigned int state;         /* 0x380 */
    char pad0384[0x22C];
    AirFlags flags;             /* 0x5B0 */
    char pad05B8[0xAC8];
    unsigned char airTime;      /* 0x1080 */
    unsigned char airTimeMax;   /* 0x1081 */
};

int func_088377C0(FighterAir *fighter);

struct FighterAirOps {
    static unsigned char airborne(FighterAir *fighter)
    {
        return (fighter->state & 0x200) != 0;
    }
    static void setHeld(AirFlags *flags)
    {
        ((unsigned char *)flags)[1] |= 0x10;
    }
    static void clearHeld(AirFlags *flags)
    {
        ((unsigned char *)flags)[1] &= ~0x10;
    }
};

void func_08B2E114(FighterAir *fighter)
{
    if (func_088377C0(fighter) != 0) {
        if (FighterAirOps::airborne(fighter)) {
            fighter->airTime++;
            if (fighter->airTime > fighter->airTimeMax) {
                fighter->airTimeMax = fighter->airTime;
            }
            FighterAirOps::setHeld(&fighter->flags);
        }
    } else {
        fighter->airTime = 0;
        FighterAirOps::clearHeld(&fighter->flags);
    }
}

}
