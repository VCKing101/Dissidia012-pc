// If the first scene object answers the key test (slot 1, key D_08B0CDA8) sets the flag byte at +0x90; unless the state at +0x88 is 1 moves to state 0xF via func_08B081D4; returns 0.
struct Scene57 {
    char data[0x34];
    virtual int slot0(unsigned long long k);
    virtual int slot1(unsigned long long k);
};
extern "C" {
extern Scene57 *func_088729A0(int);
extern void func_08B081D4(void *, int);
extern unsigned long long D_08B0CDA8;
}
struct State57 {
    int v;
    static bool is1(int v) { return v == 1; }
};
struct Self57 {
    char pad0[0x88];
    int state;
    char pad1[4];
    unsigned char flag;
};
extern "C" int func_08B07818(Self57 *self)
{
    if (func_088729A0(0)->slot1(D_08B0CDA8) != 0) {
        int one = 1;
        self->flag = one;
    }
    if (!State57::is1(self->state)) {
        func_08B081D4(self, 0xF);
    }
    return 0;
}
