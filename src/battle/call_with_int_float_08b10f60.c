/* Calls func_08873D34 with the int at +4 of the message converted to float; returns 0. */
extern void func_08873D34(float a, int b, int c, int d, int e, int f);
int func_08B10F60(int a0, int *msg) {
    func_08873D34((float)msg[1], 2, 0, 0, 0, 0);
    return 0;
}
