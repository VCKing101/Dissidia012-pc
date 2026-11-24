/* Calls the installed handler, or the default one when none is set. */
extern void (*D_089DA334)(int a);
extern void func_089748D0(int a);
void func_08974940(int a)
{
    if (D_089DA334 != 0) {
        D_089DA334(a);
    } else {
        func_089748D0(a);
    }
}
