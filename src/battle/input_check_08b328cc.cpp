// Checks a fighter input condition: handle state 0x3 via func_08A64244, then, unless func_08B340B0
// reports mode 1, a virtual key test on the animation object (slot 1, key D_08B3ACB8).
struct Anim08B328CC {
    char data[0x34];
    virtual int slot0(unsigned long long k);
    virtual int slot1(unsigned long long k);
};
extern "C" int func_08A64244(void *, int);
extern "C" int func_08B340B0(void *);
struct Owner08B328CC {
    char pad0[0x5A0];
    Anim08B328CC *anim;
    char pad1[0x20];
    void *handle;
    void *H() { return handle; }
};
extern "C" {
extern unsigned long long D_08B3ACB8;
unsigned char func_08B328CC(Owner08B328CC *self)
{
    int t = 0;
    if (self->handle != 0) {
        int found = func_08A64244(self->H(), 0x3);
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
        int hit = self->anim->slot1(D_08B3ACB8);
        return hit != 0 || r;
    }
}
}
