/* Calls func_08B555EC(self, i, word) for the two words starting at +0x20. */
extern "C" void func_08B555EC(int *self, int i, int v);
extern "C" void func_08B55594(int *self)
{
    for (int i = 0, *p = self; i < 2; i++, p++) {
        func_08B555EC(self, i, p[0x20 / 4]);
    }
}
