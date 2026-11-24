/* Calls the function pointer stored in a global and ignores its result. */
extern int (*D_089DA328)(int a);
void func_089748A4(int a)
{
    int r = D_089DA328(a);
    if (r == 0) {
        r = 1;
    }
}
