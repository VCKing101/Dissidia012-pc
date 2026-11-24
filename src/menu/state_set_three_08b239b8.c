/* Stores three words at 0x09B6DA58 and calls the 0x088650A4 hook on self + 0x100. */
extern int D_09B6DA58[3];
extern void func_088650A4(char *p);
void func_08B239B8(char *self, int a, int b, int c)
{
    D_09B6DA58[0] = a;
    D_09B6DA58[1] = b;
    D_09B6DA58[2] = c;
    func_088650A4(self + 0x100);
}
