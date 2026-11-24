/* Return the active 0x154-byte entry when it passes its check, else 0. */
extern int func_0892A1E0(int entry);
int func_0892A570(int self)
{
    int r = 0;
    if (*(int *)(self + 0x6A4) >= 0 && func_0892A1E0(self + *(int *)(self + 0x6A4) * 0x154) == 1) {
        r = self + *(int *)(self + 0x6A4) * 0x154;
    }
    return r;
}
