// Fighter input check: 0 while the lock flag (+0x5B7 bit 8) is set; otherwise state 0x1E on the handle at +0x5C4, then (unless func_08B340B0 reports mode 1) a virtual key test on the animation object (slot 1, key D_08B3AC50).
struct Anim08B31C2C {
    char data[0x34];
    virtual int slot0(unsigned long long k);
    virtual int slot1(unsigned long long k);
};
extern "C" int func_08A64260(void *, int);
extern "C" int func_08B340B0(void *);
struct Flags08B31C2C {
    char pad0[7];
    unsigned char b7;
    static bool locked(Flags08B31C2C *f) { return (f->b7 & 8) != 0; }
};
struct Owner08B31C2C {
    char pad0[0x5A0];
    Anim08B31C2C *anim;
    char pad1[0xC];
    Flags08B31C2C flags;
    char pad2[0x5C4 - 0x5B8];
    void *handle;
    void *H() { return handle; }
};
extern "C" {
extern unsigned long long D_08B3AC50;
int func_08B31C2C(Owner08B31C2C *self)
{
    Flags08B31C2C *fl = &self->flags;
    if (Flags08B31C2C::locked(fl)) {
        return 0;
    }
    int t = 0;
    if (self->handle != 0) {
        if (func_08A64260(self->H(), 0x1E) != 0) {
            t = 1;
        }
    }
    unsigned char r = t;
    if (func_08B340B0(self) == 1) {
        return r;
    }
    {
        int hit = self->anim->slot1(D_08B3AC50);
        return hit != 0 || r;
    }
}
}
