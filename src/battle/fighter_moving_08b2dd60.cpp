extern "C" {

struct MoveFlags {
    unsigned char bytes[4];
};

struct MoveInfo {
    int field_00;
    unsigned int flags;         /* 0x04 */
};

struct FighterMove {
    char pad000[0xB0];
    float field_B0;
    float field_B4;
    float field_B8;
    float speed;                /* 0xBC */
    char pad0C0[0xD0];
    MoveFlags flags;            /* 0x190 */
    char pad194[0x210];
    MoveInfo *info;             /* 0x3A4 */
};

int func_088377C0(FighterMove *fighter);

struct FighterMoveOps {
    static unsigned char forced(unsigned char *flags)
    {
        return (flags[2] & 0x10) != 0;
    }
};

int func_08B2DD60(FighterMove *fighter)
{
    if (fighter->speed <= 0.0f || (fighter->field_B0 == 0.0f && fighter->field_B8 == 0.0f)) {
        return 0;
    }
    if (func_088377C0(fighter) != 0 && !FighterMoveOps::forced(fighter->flags.bytes)) {
        return 0;
    }
    return (fighter->info->flags & 0x20) != 0;
}

}
