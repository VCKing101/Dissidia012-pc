extern void func_08912BC0(int *state, int a, int b);
extern int *D_089D83B8;

void func_08912D4C(int a, int b)
{
    if (*D_089D83B8 != 0) {
        return;
    }
    func_08912BC0(D_089D83B8, a, b);
}
