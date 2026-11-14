/* 1 when the unit has no info, is hidden (flag 4) or has status bit 7. */
struct Status08837948 {
    unsigned char bits;

    static bool flag(Status08837948 *s) { return (s->bits & 0x80) != 0; }
};

struct Unit08837948 {
    char pad_000[0x190];
    Status08837948 status;
    char pad_191[0x1EF];
    unsigned int flags;
    char pad_384[0x20];
    void *info;

    static bool has(Unit08837948 *u, unsigned int mask) { return (u->flags & mask) != 0; }
};

extern "C" {

unsigned char func_08837948(Unit08837948 *u)
{
    Status08837948 *status;
    int result = 0;
    if (u->info == 0 || Unit08837948::has(u, 4) || (status = &u->status, Status08837948::flag(status))) {
        result = 1;
    }
    return result;
}

}
