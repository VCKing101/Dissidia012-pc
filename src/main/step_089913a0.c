/* Steps a two-value interpolator (words 3 and 4 advance by word 7) and snaps both to word 5 when the countdown at word 6 ends; returns 1 at the end. */
int func_089913A0(int *p) {
    int n = p[6] - 1;
    p[6] = n;
    if (n != 0) {
        int a = p[3];
        int step = p[7];
        int b = p[4];
        p[3] = a + step;
        p[4] = b + step;
        return 0;
    }
    int t = p[5];
    p[3] = t;
    p[4] = t;
    return 1;
}
