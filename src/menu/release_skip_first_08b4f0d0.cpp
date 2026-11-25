// Releases the non-zero handles at self+8..self+0x14 (slot 0 skipped) with func_088DA060 and clears them.
extern "C" void func_088DA060(int h);
extern "C" void func_08B4F0D0(int *self)
{
    int i = 0;
    int *p = self;
    for (; i < 5; i++, p++) {
        if (i == 0) { continue; }
        if (p[1] != 0) {
            func_088DA060(p[1]);
            p[1] = 0;
        }
    }
}
