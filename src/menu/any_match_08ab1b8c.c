/* Returns 1 when any of the count entries (stride 0x16DC from +0x1588) passes both checks, otherwise 0. */
extern int func_0892A1E0(int p);
extern int func_08AB1B4C(int self, int p);
int func_08AB1B8C(int self, int base, int count) {
    int res = 0;
    int i = 0;
    if (i < count) {
        int p = base + 0x1588;
        do {
            int r = func_0892A1E0(p);
            if (r != 0) {
                r = func_08AB1B4C(self, p);
                if (r != 0) {
                    res = 1;
                    break;
                }
            }
            i = i + 1;
            p = p + 0x16DC;
        } while (i < count);
    }
    return res;
}
