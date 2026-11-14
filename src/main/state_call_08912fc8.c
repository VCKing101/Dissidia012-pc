extern void func_08912DC4(int *state, int a, int b);
extern int *D_089D83C0;

void func_08912FC8(int a, int b)
{
    if (*D_089D83C0 != 0) {
        return;
    }
    func_08912DC4(D_089D83C0, a, b);
}
