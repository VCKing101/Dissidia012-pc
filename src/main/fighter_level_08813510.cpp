/* Resolves a fighter to its controlling fighter and returns its level, or 0
   when flag 0x800000 is set. */
struct Stats08813510 {
    char pad_00[0x14];
    short level;
};

struct Fighter08813510 {
    char pad_000[0x384];
    unsigned int flags;
    char pad_388[0x1A4];
    Stats08813510 *stats;
    char pad_530[0x12];
    unsigned char kind;
    char pad_543[0x91];
    Fighter08813510 *owner;
    char pad_5D8[0xB30];
    Fighter08813510 *partner;

    static bool is_kind(Fighter08813510 *f, int kind) { return f->kind == kind; }
    static bool has_flag(Fighter08813510 *f, unsigned int flag) { return (f->flags & flag) != 0; }
};

extern "C" {

int func_08813510(Fighter08813510 *fighter)
{
    if (Fighter08813510::is_kind(fighter, 3)) {
        fighter = fighter->owner;
    }
    if (Fighter08813510::is_kind(fighter, 1)) {
        fighter = fighter->partner;
    }
    int result = 0;
    if (!Fighter08813510::has_flag(fighter, 0x800000)) {
        result = fighter->stats->level;
    }
    return result;
}

}
