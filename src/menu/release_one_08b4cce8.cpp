/* Releases the handle at self+0x10 with func_088DA060 if set and clears it. */
extern "C" void func_088DA060(int h);
extern "C" void func_08B4CCE8(int *self)
{
    for (int i = 0, *p = self; i < 1; i++, p++) {
        if (p[0x10 / 4] != 0) {
            func_088DA060(p[0x10 / 4]);
            p[0x10 / 4] = 0;
        }
    }
}
