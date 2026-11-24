/* Looks up a pair of values from a six-entry table indexed by a short. */
void func_08B0F188(int a0, int *out0, int *out1, short idx) {
    int t[6];

    int one = 1;
    int two = 2;
    t[0] = 0; t[1] = one; t[2] = 0; t[3] = one; t[4] = two; t[5] = two;
    *out0 = t[idx];
    *out1 = t[idx + 3];
}
