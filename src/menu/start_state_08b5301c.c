/* Runs func_08B532D0(self), then func_08B53370 with the four arguments, and sets the state word at +0x28 to 1. */
extern void func_08B532D0(int *self);
extern void func_08B53370(int *self, int a, int b, int c, int d);
void func_08B5301C(int *self, int a, int b, int c, int d)
{
    func_08B532D0(self);
    func_08B53370(self, a, b, c, d);
    self[0x28 / 4] = 1;
}
