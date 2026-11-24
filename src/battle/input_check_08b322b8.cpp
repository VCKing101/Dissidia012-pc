// Fighter input check: returns 0 when func_08B34E38 is set; otherwise state 0x1C via func_08A64260 on the handle at +0x5C4, then (unless func_08B340B0 reports mode 1) a virtual key test on the animation object (slot 1, key D_08B3AC80).
struct Anim08B322B8 {
    char data[0x34];
    virtual int slot0(unsigned long long k);
    virtual int slot1(unsigned long long k);
};
extern "C" int func_08A64260(void *, int);
extern "C" int func_08B34E38(void *);
extern "C" int func_08B340B0(void *);
struct Owner08B322B8 {
    char pad0[0x5A0];
    Anim08B322B8 *anim;
    char pad1[0x20];
    void *handle;
    void *H() { return handle; }
};
extern "C" {
extern unsigned long long D_08B3AC80;
unsigned char func_08B322B8(Owner08B322B8 *self)
{
    if (func_08B34E38(self) != 0) {
        return 0;
    }
    int t = 0;
    if (self->handle != 0) {
        int found = func_08A64260(self->H(), 0x1C);
        int one = 1;
        if (found == one) {
            t = 1;
        }
    }
    unsigned char r = t;
    if (func_08B340B0(self) == 1) {
        return r;
    }
    {
        int hit = self->anim->slot1(D_08B3AC80);
        return hit != 0 || r;
    }
}
}
