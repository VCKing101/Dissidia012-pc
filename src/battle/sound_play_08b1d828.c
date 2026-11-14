extern int D_08B1D878[];
extern void func_088A24D4(int a, int id, int b, int c);

int func_08B1D828(unsigned int index)
{
    if (index >= 11) {
        return -1;
    }
    func_088A24D4(0, D_08B1D878[index], 1, 0);
    return 0;
}
