// Calls the four stage functions 08B3BD28, 08B3BD9C, 08B3BE10 and 08B3BE84 on the same object and floats.
extern void func_08B3BD28(int a, float f);
extern void func_08B3BD9C(int a, float f, int b, int c);
extern void func_08B3BE10(int a, float f, int b, int c);
extern void func_08B3BE84(int a, float f, int b, float g);
void func_08B3BFA0(float f, float g, int a, int b, int c, int d, int e)
{
    int le = e;
    int lb = b;
    int ld = d;
    int la = a;
    func_08B3BD28(la, g);
    func_08B3BD9C(la, g, lb, ld);
    func_08B3BE10(la, g, lb, le);
    func_08B3BE84(la, f, lb, g);
}
