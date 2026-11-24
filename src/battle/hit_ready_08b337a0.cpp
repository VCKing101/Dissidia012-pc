// True when the fighter is not stunned/guarding, no global lock is set, its gauge (+0x18) is at least 10000 and flag 0x80 of byte 1 is clear.
struct Flags57 {
    char pad0[1];
    unsigned char b1;
    unsigned char b2;
    static bool lockedB2(Flags57 *f) { return (f->b2 & 4) != 0; }
    static bool bit80(Flags57 *f) { return (f->b1 & 0x80) != 0; }
};
struct Mode57 {
    char pad0[8];
    unsigned char bits;
    static bool stun(Mode57 *m) { return (m->bits & 2) != 0; }
};
struct Gauge57 {
    char pad0[0x18];
    float value;
    static bool full(Gauge57 *g) { return g->value >= 10000.0f; }
    static bool has(Gauge57 *g) { return g != 0; }
};
struct Global57 {
    char pad0[0x278];
    int lock;
    static bool locked(Global57 *g) { return g->lock != 0; }
};
extern Global57 *D_08A405E0;
struct Fighter57 {
    char pad0[0x2F8];
    Mode57 *mode;
    char pad1[0x52C - 0x2FC];
    Gauge57 *gauge;
    char pad2[0x5B0 - 0x530];
    Flags57 flags;
    static unsigned char run(Fighter57 *self)
    {
        int r = 0;
        if (!Mode57::stun(self->mode)) {
            Flags57 *fl = &self->flags;
            if (!Flags57::lockedB2(fl)) {
                if (!Global57::locked(D_08A405E0)) {
                    if (Gauge57::has(self->gauge)) {
                        if (Gauge57::full(self->gauge)) {
                            if (!Flags57::bit80(fl)) {
                                r = 1;
                            }
                        }
                    }
                }
            }
        }
        return r;
    }
};
extern "C" unsigned char func_08B337A0(Fighter57 *self) { return Fighter57::run(self); }
