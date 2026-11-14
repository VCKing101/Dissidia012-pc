/* Whether an actor in an action (or stunned) may act now: not while any
   lock state bit is set, and for the player not while guard-locked. */
struct Status088377D0 {
    unsigned char flags;

    static bool acting(Status088377D0 *s) { return (s->flags & 0x20) != 0; }
    static bool recovering(Status088377D0 *s) { return (s->flags & 0x40) != 0; }
    static unsigned char busy(Status088377D0 *s)
    {
        int r = 0;
        if (acting(s) || recovering(s)) {
            r = 1;
        }
        return r;
    }
};

struct Guard088377D0 {
    char pad_0[6];
    unsigned char flags;

    static bool locked(Guard088377D0 *g) { return (g->flags & 0x20) != 0; }
};

struct Actor088377D0 {
    char pad_000[0x190];
    Status088377D0 status;
    char pad_191[0x380 - 0x191];
    unsigned int flags;
    char pad_384[4];
    unsigned long long state;
    char pad_390[0x5B0 - 0x390];
    Guard088377D0 guard;

    static bool stunned(Actor088377D0 *a) { return (a->flags & 0x20000000) != 0; }
    static bool any(Actor088377D0 *a, unsigned long long mask) { return (a->state & mask) != 0; }
};

extern "C" {

extern unsigned long long D_089C7FB8;

int func_0883739C(Actor088377D0 *self);

unsigned char func_088377D0(Actor088377D0 *self)
{
    Status088377D0 *st = &self->status;
    int ok = 0;
    if (Status088377D0::busy(st) || Actor088377D0::stunned(self)) {
        if (!Actor088377D0::any(self, D_089C7FB8)) {
            Guard088377D0 *g;
            if (func_0883739C(self) != 1 || Guard088377D0::locked(g = &self->guard) != true) {
                ok = 1;
            }
        }
    }
    return ok;
}

}
