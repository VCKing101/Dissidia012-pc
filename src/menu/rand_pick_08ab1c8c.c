/* Advances the global LCG and returns element (rand % n) of the int array. */
extern int D_09B6F454;
int func_08AB1C8C(int *arr, int n) {
    int m = n - 1;
    return arr[(((D_09B6F454 = D_09B6F454 * 0x343FD + 0x269EC3) >> 16) & 0x7FFF) % (m + 1)];
}
