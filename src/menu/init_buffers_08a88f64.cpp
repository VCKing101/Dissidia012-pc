// Allocates and registers the "CROOM_T_BUF" block and, when the check fails, a second 0xC8000-byte "USMM_FONT" block from the global heap.
extern "C" {
extern int D_089D5EC0;
int func_0888204C(int size);
int func_0885C40C(int block, int size, const char *name, int n);
int func_08867894(void);
int func_0885C6CC(int heap, int size, int a, int b, int c, int d);
extern const char D_08A8B700[];
extern const char D_08A8B710[];
}
template <class T> struct Kept { T value; Kept(T v) : value(v) {} };
extern "C" void func_08A88F64(int self) {
    Kept<int> saved(self);
    int blk = func_0888204C(0x270800);
    *(int *)(saved.value + 0x134) = blk;
    int r = func_0885C40C(blk, 0x270800, D_08A8B700, 0x40);
    *(int *)(saved.value + 0x138) = r;
    int fl = func_08867894();
    self = saved.value;
    if (fl == 0) {
        saved.value = self;
        int one = 1;
        int h = func_0885C6CC(*(int *)(D_089D5EC0 + 0x710), 0xC8000, 0, 0, one, 0);
        *(int *)(saved.value + 0x13C) = h;
        int r2 = func_0885C40C(h, 0xC8000, D_08A8B710, 0x40);
        *(int *)(saved.value + 0x140) = r2;
    }
}
