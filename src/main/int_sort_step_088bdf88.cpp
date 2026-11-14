/* Forwards to the median/partition step with a null value-type tag. */
struct IntLess088BDF88 { signed char unused; };
extern "C" {
void func_088BDFAC(int *a, int *b, int *c, int *type, IntLess088BDF88 less);
void func_088BDF88(int *a, int *b, int *c, IntLess088BDF88 less)
{
    func_088BDFAC(a, b, c, 0, less);
}
}
