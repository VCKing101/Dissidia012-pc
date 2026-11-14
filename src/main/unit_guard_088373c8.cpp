/* 1 when the unit guards (info flag 1 or 0x10, or unit flag 0x400000) and is
   not blocked by flag 0x200. */
struct Info088373C8 {
    int field_00;
    union {
        unsigned int flags;
        struct {
            unsigned int guard : 1;
        } bits;
    } u;
};

struct Unit088373C8 {
    char pad_000[0x384];
    unsigned int flags;
    char pad_388[0x1C];
    Info088373C8 *info;

    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
    static bool guard(Info088373C8 *i) { return i->u.bits.guard; }
};

extern "C" {

unsigned char func_088373C8(Unit088373C8 *u)
{
    int result = 0;
    if ((Unit088373C8::guard(u->info) || Unit088373C8::has(u->info->u.flags, 0x10) ||
         Unit088373C8::has(u->flags, 0x400000)) &&
        !Unit088373C8::has(u->flags, 0x200)) {
        result = 1;
    }
    return result;
}

}
