/* 1 when the unit is stunned (status bit 1, flag 0x1000 or kind 3). */
struct Status0883752C {
    unsigned char bits;

    static bool stunned(Status0883752C *s) { return (s->bits & 2) != 0; }
};

struct Unit0883752C {
    char pad_000[0x190];
    Status0883752C status;
    char pad_191[0x1F3];
    unsigned int flags;
    char pad_388[0x1BA];
    unsigned char kind;

    static bool has(Unit0883752C *u, unsigned int mask) { return (u->flags & mask) != 0; }
    static bool kind_is(Unit0883752C *u, int kind) { return u->kind == kind; }
};

extern "C" {

unsigned char func_0883752C(Unit0883752C *u)
{
    Status0883752C *status = &u->status;
    return (Status0883752C::stunned(status) || Unit0883752C::has(u, 0x1000) || Unit0883752C::kind_is(u, 3)) ? 1 : 0;
}

}
