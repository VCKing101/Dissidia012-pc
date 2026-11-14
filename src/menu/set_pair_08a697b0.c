extern void func_088D4E00(unsigned int value, int target);

void func_08A697B0(int self, int value, int extra)
{
    int chosen;
    if (*(int *)(self + 0x28) == value) {
        return;
    }
    chosen = extra == -1 ? *(int *)(self + 0x34) : extra;
    *(int *)(self + 0x1D0) = chosen;
    *(int *)(self + 0x28) = value;
    func_088D4E00(value, self + 0x1B0);
}
