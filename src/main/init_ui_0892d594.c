/* Build the two handles of the panel, run the three sub-inits with the extra arguments and register them. */
extern void func_0892D838(void);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern int func_0887E534(int h);
extern void func_0892D76C(int self, int a, int b, int c);
extern void func_0892D6A0(int self, int a, int b, int c);
extern void func_0892D8F0(int self, int a, int b, int c);
extern void func_088D428C(int p, int q, void *tbl, int n);
extern char D_089CEE20[];
void func_0892D594(int self0, int a0, int b, int c, int d, int e)
{
    int self = self0;
    int a = a0;
    int h;
    func_0892D838();
    h = func_0887B3EC(a, b, 0, 2, 0, 0);
    *(int *)(self + 0x44) = h;
    h = func_0887E534(h);
    *(int *)(self + 0x48) = h;
    func_0892D76C(self, a, b, c);
    h = func_0887B3EC(a, b, 0, 3, 0, 0);
    *(int *)(self + 0x54) = h;
    h = func_0887E534(h);
    *(int *)(self + 0x58) = h;
    func_0892D6A0(self, a, b, d);
    func_0892D8F0(self, a, b, e);
    func_088D428C(self + 0x30, self + 0x44, D_089CEE20, 1);
}
