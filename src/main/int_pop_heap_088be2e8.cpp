/* std::pop_heap over an int range (STLport __pop_heap_aux). */
struct IntLess088BE2E8 { signed char unused; };
extern "C" {
void func_088BE1BC(int *first, int hole, int len, int value, IntLess088BE2E8 less);
}
struct Heap088BE2E8 {
    static void pop(int *first, int *last, int *result, int value, IntLess088BE2E8 less)
    {
        *result = *first;
        func_088BE1BC(first, 0, last - first, value, less);
    }
    static void popAux(int *first, int *last, int *type, IntLess088BE2E8 less)
    {
        pop(first, last - 1, last - 1, *(last - 1), less);
    }
};
extern "C" {
void func_088BE2E8(int *first, int *last, IntLess088BE2E8 less)
{
    Heap088BE2E8::popAux(first, last, 0, less);
}
}
