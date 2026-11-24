/* Runs func_08B53984(self), then func_08B53A24 with the three arguments, func_088D4BF4(8) and sets the state word at +0x28 to 1. */
extern void func_08B53984(int *self);
extern void func_08B53A24(int *self, int a, int b, int c);
extern void func_088D4BF4(int v);
void func_08B53734(int *self, int a, int b, int c)
{
    func_08B53984(self);
    func_08B53A24(self, a, b, c);
    func_088D4BF4(8);
    self[0x28 / 4] = 1;
}
