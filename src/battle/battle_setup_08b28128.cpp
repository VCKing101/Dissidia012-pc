// Prepares battle state (variant with an extra start call): refreshes the global blocks, clears a bit on three overlay objects, zeroes the flags and sets two status bits on both fighters, returns 0.
struct BsFighter {
    char pad0[0x5B0];
    unsigned char b5B0[8];
    char pad5B8[0xC];
    int h5C4;
};
struct BsList { char pad0[0x38]; BsFighter *f[2]; };
struct BsGlobal {
    char pad0[0x4C8];
    unsigned char f4C8;
    char pad4C9[0x2DF];
    unsigned char f7A8;
    char pad7A9[0x6287];
    unsigned char f6A30;
};
struct BsOverlay { char pad0[0x14]; unsigned char f14; };
struct BsSelf { int pad0; int p4; };
extern "C" {
extern void func_088D1118(int a);
extern void func_08B214A0(int a);
extern void func_08B214FC(int a);
extern void func_088D71F0(void *p);
extern void func_088CD988(void *p);
extern void func_088CE6D0(void *p);
extern void func_08B187B8(int a);
extern void func_0888A0C4(void);
extern void func_08ACBEC4(void *p, int a, int b);
extern void func_08ACF964(void *p, int a, int b);
extern int func_08A642BC(int h);
extern void func_08A7EBF4(int h, int a);
extern int D_089D7830;
extern int D_08B18830;
extern BsGlobal *D_089D4DC8;
extern BsList *D_089D6A10;
extern BsOverlay *D_09B491E8[3];
extern char D_08AEB9C0;
int func_08B28128(BsSelf *self)
{
    BsGlobal *g;
    func_088D1118(D_089D7830);
    if (self->p4 != 0) {
        func_08B214A0(self->p4);
        func_08B214FC(self->p4);
    }
    func_088D71F0((char *)D_089D6A10 + 0x230);
    func_088CD988((char *)D_089D6A10 + 0x6C);
    func_088CE6D0((char *)D_089D6A10 + 0x728);
    func_08B187B8(D_08B18830);
    for (int i = 0; i < 3; i++) {
        if (D_09B491E8[i] != 0) {
            D_09B491E8[i]->f14 = D_09B491E8[i]->f14 & -0x21;
        }
    }
    func_0888A0C4();
    g = D_089D4DC8;
    int z1 = 0;
    int z2 = 0;
    g->f7A8 = z1;
    g->f4C8 = z2;
    int z3 = 0;
    g->f6A30 = z3;
    D_08AEB9C0 = 0;
    for (int i = 0; i < 2; i++) {
        BsFighter *f = D_089D6A10->f[i];
        if (f == 0) {
            continue;
        }
        func_08ACBEC4((char *)f + 0x6E0, 0, 1);
        func_08ACF964((char *)f + 0x760, 0, 1);
        {
            unsigned char *b = &f->b5B0[0];
            b[5] = b[5] | 4;
            b[5] = b[5] | 0x40;
        }
        {
            int h = f->h5C4;
            if (h != 0) {
                func_08A7EBF4(func_08A642BC(h), 0);
            }
        }
    }
    return 0;
}
}
