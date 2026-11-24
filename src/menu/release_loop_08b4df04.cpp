/* For the 19 handles at self+0x40, releases each non-zero one with func_0887BA5C and clears it. */
extern "C" void func_0887BA5C(int h);
extern "C" void func_08B4DF04(int *self)
{
    for (int i = 0, *p = self; i < 19; i++, p++) {
        if (p[0x40 / 4] != 0) {
            func_0887BA5C(p[0x40 / 4]);
            p[0x40 / 4] = 0;
        }
    }
}
