// Fighter input check: 0 when lock flag (+0x5B7 bit 2), func_08B3406C or +0x5B2 gating applies; else state 0xC on handle, then virtual key test D_08B3AC30.
struct Anim08B31108 {
    char data[0x34];
    virtual int slot0(unsigned long long k);
    virtual int slot1(unsigned long long k);
};
extern "C" int func_08A64260(void *, int);
extern "C" int func_08B340B0(void *);
extern "C" int func_08B3406C(void *);
struct Flags08B31108 {
    char pad0[2];
    unsigned char b2;
    char pad1[4];
    unsigned char b7;
    static bool locked(Flags08B31108 *f) { return (f->b7 & 2) != 0; }
    static bool g4(Flags08B31108 *f) { return (f->b2 & 4) != 0; }
};
struct Sub08B31108 {
    char pad[0xC];
    unsigned int w;
    static bool g8000(Sub08B31108 *s) { return (s->w & 0x8000) != 0; }
};
struct Owner08B31108 {
    char pad0[0x5A0];
    Anim08B31108 *anim;
    Sub08B31108 *sub;
    char pad1[0x8];
    Flags08B31108 flags;
    char pad2[0x5C4 - 0x5B8];
    void *handle;
    void *H() { return handle; }
};
extern "C" {
extern unsigned long long D_08B3AC30;
int func_08B31108(Owner08B31108 *self)
{
    Flags08B31108 *fl1 = &self->flags;
    if (Flags08B31108::locked(fl1)) {
        return 0;
    }
    if (func_08B3406C(self) != 0) {
        return 0;
    }
    Flags08B31108 *fl2 = &self->flags;
    if (!Flags08B31108::g4(fl2) && self->sub != 0 && !Sub08B31108::g8000(self->sub)) {
        return 0;
    }
    int t = 0;
    if (self->handle != 0) {
        if (func_08A64260(self->H(), 0xC) == 1) {
            t = 1;
        }
    }
    unsigned char r = t;
    if (func_08B340B0(self) == 1) {
        return r;
    }
    {
        int hit = self->anim->slot1(D_08B3AC30);
        return hit != 0 || r;
    }
}
}
