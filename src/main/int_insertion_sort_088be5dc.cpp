/* Unguarded insertion sort over an int range (STLport). */
struct IntLess088BE5DC { signed char unused; };
extern "C" {
void func_088BE58C(int *last, int value, IntLess088BE5DC less);
void func_088BE5DC(int *first, int *last, int *type, IntLess088BE5DC less)
{
    for (int *i = first; i != last; ++i) {
        func_088BE58C(i, *i, less);
    }
}
}
