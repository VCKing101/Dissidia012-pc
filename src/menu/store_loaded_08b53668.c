/* Calls a loader on the global manager with three forwarded arguments and stores the result at +0x2C. */
extern int D_089D7830;
extern void func_0892AC5C(void);
extern int func_088D0700(int mgr, unsigned char a, int b, int c);
void func_08B53668(int *self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    int b = a4;
    func_0892AC5C();
    int x = b & 0xFF;
    self[0x2C / 4] = func_088D0700(D_089D7830, x, a5, a6);
}
