/* For the 3 handles at self+0x30, releases each non-zero one with func_088DA060 and clears it. */
extern "C" void func_088DA060(int h);
extern "C" void func_08B4F754(int *self)
{
    for (int i = 0, *p = self; i < 3; i++, p++) {
        if (p[0x30 / 4] != 0) {
            func_088DA060(p[0x30 / 4]);
            p[0x30 / 4] = 0;
        }
    }
}
