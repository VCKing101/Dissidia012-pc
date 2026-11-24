// Returns the entry's id, or when it is -1 a random one from the byte list at +0x111; stores the id byte at +0x1585.
extern "C" {
extern int D_09B6F454;
int *func_08AB0F80(int self, int a, int b);
}
struct Pick {
    static int Value(int self) {
        int *p = func_08AB0F80(self, 4, *(char *)(self + 4));
        int v = *p;
        int r;
        if (v != -1) {
            r = v;
        } else {
            int m = *(char *)(self + 0xA) - 2;
            int k;
            D_09B6F454 = D_09B6F454 * 0x343FD + 0x269EC3;
            k = ((D_09B6F454 >> 16) & 0x7FFF) % (m + 1);
            r = *(char *)(k + self + 0x111);
            v = *p;
        }
        *(char *)(self + 0x1585) = v;
        return r;
    }
};
extern "C" int func_08AB1610(int self) {
    return Pick::Value(self);
}
