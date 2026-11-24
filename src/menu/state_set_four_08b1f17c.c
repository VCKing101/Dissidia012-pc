/* Stores four words at 0x09B6DA30 and calls the 0x088650A4 hook on self + 0x100. */
extern int D_09B6DA30[4];
extern void func_088650A4(char *p);
void func_08B1F17C(char *self, int a, int b, int c, int d)
{
    D_09B6DA30[0] = a;
    D_09B6DA30[1] = b;
    D_09B6DA30[2] = c;
    D_09B6DA30[3] = d;
    func_088650A4(self + 0x100);
}
