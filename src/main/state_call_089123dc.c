extern void func_08911FEC(int *state, int a, int b);
extern int *D_089D83B0;

void func_089123DC(int a, int b)
{
    if (*D_089D83B0 != 0) {
        return;
    }
    func_08911FEC(D_089D83B0, a, b);
}
