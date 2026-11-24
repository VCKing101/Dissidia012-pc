/* For the 12 handles at self+0x10, releases each non-zero one with func_088DA060 and clears it. */
extern "C" void func_088DA060(int h);
extern "C" void func_08B4DAF4(int *self)
{
    for (int i = 0, *p = self; i < 12; i++, p++) {
        if (p[0x10 / 4] != 0) {
            func_088DA060(p[0x10 / 4]);
            p[0x10 / 4] = 0;
        }
    }
}
