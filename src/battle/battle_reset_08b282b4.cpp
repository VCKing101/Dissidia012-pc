/* Resets battle state: flags on the global block and both fighters (clears two status bits, refreshes their helpers), returns 0. */
struct BrFighter {
    char pad0[0x5B0];
    unsigned char b5B0[8];
    char pad5B8[0xC];
    int h5C4;
};
struct BrList { char pad0[0x38]; BrFighter *f[2]; };
struct BrGlobal {
    char pad0[0x4C8];
    unsigned char f4C8;
    char pad4C9[0x2DF];
    unsigned char f7A8;
    char pad7A9[0x6287];
    unsigned char f6A30;
};
struct BrSelf { int pad0; int p4; };
extern "C" void func_088D1170(int a);
extern "C" void func_08B214DC(int a);
extern "C" void func_0888A0FC(void);
extern "C" void func_08ACBEC4(void *p, int a, int b);
extern "C" void func_08ACF964(void *p, int a, int b);
extern "C" int func_08A642BC(int h);
extern "C" void func_08A7EBF4(int h, int a);
extern "C" int D_089D7830;
extern "C" BrGlobal *D_089D4DC8;
extern "C" BrList *D_089D6A10;
extern "C" char D_08AEB9C0;

extern "C" int func_08B282B4(BrSelf *self)
{
    BrGlobal *g;
    func_088D1170(D_089D7830);
    if (self->p4 != 0) {
        func_08B214DC(self->p4);
    }
    func_0888A0FC();
    g = D_089D4DC8;
    g->f7A8 = 1;
    g->f4C8 = 1;
    g->f6A30 = 1;
    D_08AEB9C0 = 1;
    for (int i = 0; i < 2; i++) {
        BrFighter *f = D_089D6A10->f[i];
        if (f == 0) {
            continue;
        }
        func_08ACBEC4((char *)f + 0x6E0, 1, 1);
        func_08ACF964((char *)f + 0x760, 1, 1);
        {
            unsigned char *b = &f->b5B0[0];
            b[5] = b[5] & -5;
            b[5] = b[5] & -0x41;
        }
        {
            int h = f->h5C4;
            if (h != 0) {
                func_08A7EBF4(func_08A642BC(h), 1);
            }
        }
    }
    return 0;
}
