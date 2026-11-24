// Refines the phase from func_08B18838: when it is 1, drops to 0 if the fighter is flagged 0x10 or func_08ACAA74 reports 1, else to 3 unless byte 0xBC of the second entry is 1.
extern "C" {
extern int func_08B18838(void *);
extern int func_08ACAA74(void *);
}
struct FlagsB57 {
    char pad0[2];
    unsigned char b2;
    static bool f10(FlagsB57 *f) { return (f->b2 & 0x10) != 0; }
};
struct Fighter57 {
    char pad0[0x5B0];
    FlagsB57 flags;
    char pad1[0x784 - 0x5B3];
    char sub[1];
};
struct Entry57 {
    Fighter57 *f;
    int pad;
    char pad2[0xBC - 8];
    unsigned char state;
};
struct Ctx57 {
    Entry57 *e;
};
extern "C" int func_08B18A2C(void *self, Ctx57 *ctx)
{
    int phase = func_08B18838(self);
    if (phase == 1) {
        Entry57 *e = ctx->e;
        Fighter57 *f = e->f;
        e = (Entry57 *)((char *)e + 8);
        FlagsB57 *fl = &f->flags;
        char *sub = f->sub;
        if (FlagsB57::f10(fl) == 1 || func_08ACAA74(sub) == 1) {
            phase = 0;
        } else if (e->state != 1) {
            phase = 3;
        }
    }
    return phase;
}
