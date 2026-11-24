/* Release the old handle, create a new one from the arguments and register it. */
extern void func_0887BA5C(void);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern void func_088D428C(int p, int q, void *tbl, int n);
extern char D_089CF2E8[];
void func_08932000(int self0, int a, int b, int c)
{
    int self = self0;
    int h;
    if (*(int *)(self + 0x20) != 0) {
        func_0887BA5C();
        *(int *)(self + 0x20) = 0;
    }
    h = func_0887B3EC(a, b, 0, c, 0, 0);
    *(int *)(self + 0x20) = h;
    func_088D428C(self + 0x10, self + 0x18, D_089CF2E8, 1);
}
