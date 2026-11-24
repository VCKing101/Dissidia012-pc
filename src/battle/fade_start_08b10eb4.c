/* Starts a timed effect from func_088D4C5C with callback func_08B113DC, resets the tint to (0, 0, 0, 0); returns 0. */
extern int func_088D4C5C(void);
extern void func_08873D34(float t, int a, void *cb, int b, int c, int d);
extern void func_08873DB0(float *col, int a);
extern void func_08B113DC(void);
int func_08B10EB4(int a0) {
    float col[4];
    func_08873D34((float)func_088D4C5C(), 2, (void *)func_08B113DC, a0, 0, 2);
    col[0] = 0.0f;
    col[1] = 0.0f;
    col[2] = 0.0f;
    col[3] = 0.0f;
    func_08873DB0(col, 2);
    return 0;
}
