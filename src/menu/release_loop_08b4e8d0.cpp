/* For the 2 handles at self+0x2c, releases each non-zero one with func_088DAB44 and clears it. */
extern "C" void func_088DAB44(int h);
extern "C" void func_08B4E8D0(int *self)
{
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0x2c / 4] != 0) {
            func_088DAB44(p[0x2c / 4]);
            p[0x2c / 4] = 0;
        }
    }
}
