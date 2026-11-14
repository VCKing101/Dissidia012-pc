/* Builds a six-field descriptor and passes it on. */
struct Desc088C1044 {
    int a;
    int b;
    int c;
    int d;
    unsigned char e;
    unsigned char f;

    Desc088C1044(int a_, int b_, int c_, int d_, unsigned char e_, unsigned char f_) : a(a_), b(b_), c(c_), d(d_), e(e_), f(f_) {}
};
extern "C" {
void func_088C1234(int x, int y, Desc088C1044 *desc);
void func_088C1044(int x, int y, int a, int b, int c, int d, unsigned char e, unsigned char f)
{
    Desc088C1044 desc(a, b, c, d, e, f);
    func_088C1234(x, y, &desc);
}
}
