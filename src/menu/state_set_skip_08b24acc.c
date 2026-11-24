/* Stores words at 0x09B6F458, +8 and +12 and calls the 0x088650A4 hook on self + 0x100. */
extern int D_09B6F458[4];
extern void func_088650A4(char *p);
void func_08B24ACC(char *self, int a, int b, int c)
{
    D_09B6F458[0] = a;
    D_09B6F458[2] = b;
    D_09B6F458[3] = c;
    func_088650A4(self + 0x100);
}
