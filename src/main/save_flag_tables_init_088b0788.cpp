/* Save module init: clears the 128-word and 32-word flag tables at the flag block (D_089D726C) and sets the always-on bits. */
extern unsigned int *D_089D726C;
struct FlagOps {
    inline void set(int idx, unsigned int m) { unsigned int *q = D_089D726C + idx; *q |= m; }
    inline void clear(int idx, unsigned int m) { unsigned int *q = D_089D726C + idx; *q &= m; }
};
extern unsigned char D_089D5F20;
extern "C" void func_088B0788(void)
{
    FlagOps ops;
    int i;
    unsigned int *p;
    i = 1;
    p = 0;
    p = D_089D726C;
    for (i = 0; i < 0x80; i++) {
        *p++ = 0;
    }
    p = D_089D726C + 0x80;
    for (i = 0; i < 0x20; i++) {
        *p++ = 0;
    }
    ops.set(0, 0x2);
    ops.set(0, 0x4000);
    ops.set(0x15, 0x10000000);
    ops.set(0x15, 0x40000000);
    ops.set(0x16, 0x1);
    ops.set(0x16, 0x4);
    ops.set(0x16, 0x10);
    ops.set(0x16, 0x40);
    ops.set(0x16, 0x100);
    ops.set(0x16, 0x400);
    ops.set(0x16, 0x1000);
    ops.set(0x16, 0x4000);
    ops.set(0x16, 0x40000);
    ops.set(0x16, 0x100000);
    ops.set(0x16, 0x200000);
    ops.set(0x16, 0x400000);
    ops.set(0x16, 0x1000000);
    ops.set(0x16, 0x2000000);
    ops.set(0x70, 0x20000);
    ops.set(0x70, 0x80000);
    ops.set(0x70, 0x200000);
    ops.set(0x70, 0x800000);
    ops.set(0x70, 0x2000000);
    ops.set(0x70, 0x8000000);
    ops.set(0x70, 0x20000000);
    ops.set(0x70, 0x80000000);
    ops.set(0x71, 0x2);
    ops.set(0x71, 0x8);
    ops.set(0x71, 0x100);
    ops.set(0x71, 0x400);
    ops.set(0x71, 0x800);
    ops.set(0x71, 0x1000);
    ops.set(0x71, 0x4000);
    ops.set(0x71, 0x8000);
    if (D_089D5F20 == 1) {
        ops.clear(0x70, 0xDFFFFFFF);
    }
}
