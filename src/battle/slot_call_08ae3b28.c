/* Calls func_08AE3700 with the pointer at index i of the battle table. */
extern int *D_089D69F8;
extern void func_08AE3700(int p, int b, int c);

void func_08AE3B28(int i, int b, int c)
{
    func_08AE3700(D_089D69F8[6 + i], b, c);
}
