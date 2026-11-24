/* Advances the global LCG and returns the first word of the record picked by (rand % (count - 1)) + 1. */
extern int D_09B6F454;
extern int *func_08AB0F80(int rec, int a, int slot);
int func_08AB1C24(int self) {
    int m = *(signed char *)(self + 6) - 2;
    int s = (((D_09B6F454 = D_09B6F454 * 0x343FD + 0x269EC3) >> 16) & 0x7FFF) % (m + 1);
    return *func_08AB0F80(self, 0, s + 1);
}
