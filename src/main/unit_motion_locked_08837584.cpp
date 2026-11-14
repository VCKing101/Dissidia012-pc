/* 1 when the unit's info flag 4 is set or it plays motion 700, 701, 750 or 751. */
struct Info08837584 {
    int field_00;
    unsigned int flags;
};

struct Unit08837584 {
    char pad_000[0x180];
    short motion;
    char pad_182[0x222];
    Info08837584 *info;

    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {

unsigned char func_08837584(Unit08837584 *u)
{
    int result = 0;
    short motion;
    if (Unit08837584::has(u->info->flags, 4) ||
        (motion = u->motion, motion == 700 || motion == 701 || motion == 750 || motion == 751)) {
        result = 1;
    }
    return result;
}

}
