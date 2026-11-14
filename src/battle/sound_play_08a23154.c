extern int D_08A231A8[];
extern void func_088A24D4(int channel, int sound, int count, int flags);

int func_08A23154(unsigned int index)
{
    if (index >= 3) {
        return -1;
    }
    func_088A24D4(0, D_08A231A8[index], 1, 0);
    return 0;
}
