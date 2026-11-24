/* Calls func_08A62020 when the float at +4 of the message is outside (0.99, 1.01), otherwise func_08A620B0; returns 0. */
extern void func_08A62020(int g, int a);
extern void func_08A620B0(int g, int a);
extern int D_08A62280;
int func_08B10FD4(int a0, float *msg) {
    float *p = msg + 1;
    if (*p > 0.99f && *p < 1.01f) {
        func_08A620B0(D_08A62280, 0);
    } else {
        func_08A62020(D_08A62280, 0);
    }
    return 0;
}
