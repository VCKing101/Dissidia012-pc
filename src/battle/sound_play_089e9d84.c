extern int D_089E9DD8[];
extern void func_088A24D4(int channel, int sound, int count, int flags);

int func_089E9D84(unsigned int index)
{
    if (index >= 5) {
        return -1;
    }
    func_088A24D4(0, D_089E9DD8[index], 1, 0);
    return 0;
}
