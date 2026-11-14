/* std::sort over an int range: introsort to depth 2*log2(n), then the final insertion sort. */
struct IntLess088BDC2C { signed char unused; };
extern "C" {
void func_088BDDD0(int *first, int *last, int *type, int depth, IntLess088BDC2C less);
void func_088BE3E0(int *first, int *last, IntLess088BDC2C less);
}
struct Sort088BDC2C {
    static int lg(int n)
    {
        int k;
        for (k = 0; n != 1; n >>= 1) {
            ++k;
        }
        return k;
    }
};
extern "C" {
void func_088BDC2C(int *first, int *last, IntLess088BDC2C less)
{
    if (first != last) {
        func_088BDDD0(first, last, 0, Sort088BDC2C::lg(last - first) * 2, less);
        func_088BE3E0(first, last, less);
    }
}
}
