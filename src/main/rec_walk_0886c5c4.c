typedef struct O5 { char pad[0xC]; int h; } O5;
extern int func_089B0E3C(int h);
extern int func_089B2CCC(int h, int i, int j);
extern void func_089A0C20(int a);
extern void func_089B0D88(int h, int f, int z);
extern void func_089A12A4(int a, int b, int c, unsigned d, unsigned e);
extern void func_089A0384(int a);
extern void func_089A03DC(int a);
extern void func_089B47AC(int h, int i, int z);

void func_0886C5C4(O5 *self, int idx, unsigned char rec, unsigned col, int snd, unsigned char inv) {
    if (rec != 0) {
        int count = func_089B0E3C(self->h);
        int i;
        for (i = idx; i < count; i++) {
            if (func_089B2CCC(self->h, i, idx) != 0) {
                func_0886C5C4(self, i, 0, col, snd, inv);
            }
        }
    }
    func_089A0C20(snd);
    unsigned neg;
    unsigned a16;
    unsigned a24;
    unsigned a8;
    unsigned hi;
    unsigned lo;
    if ((int)col < 0xFF && (int)col >= 0) {
        func_089B0D88(self->h, 0x200000, 0);
        a16 = col << 16;
        a24 = col << 24;
        a8 = col << 8;
        hi = a16 | a24;
        lo = col | a8;
        col = hi | lo;
        neg = -1 - col;
        if (inv == 0) {
            func_089A12A4(0, 10, 10, col, neg);
        } else {
            func_089A12A4(0, 10, 10, col, -1);
        }
        goto done;
    } else {
        func_089B0D88(self->h, 0x200000, 0x200000);
    }
done:
    func_089A0384(0x15);
    func_089B47AC(self->h, idx, 0);
    func_089A03DC(0x15);
    func_089B0D88(self->h, 0x200000, 0x200000);
    if (snd != -1) {
        int m1 = -1;
        func_089A0C20(m1);
    }
}
