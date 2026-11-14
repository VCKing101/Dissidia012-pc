extern int D_089D4960;
extern void func_08844250(int root, int a, int b, int flags, int on);

void func_08844F50(int a, int b, unsigned char on)
{
    int flag = on != 0;

    func_08844250(D_089D4960, a, b, 0x80000, flag);
}
