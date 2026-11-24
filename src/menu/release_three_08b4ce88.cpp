/* For the three handles at self+0x14, releases each non-zero one with func_0887BA5C and clears it. */
extern "C" void func_0887BA5C(int h);
extern "C" void func_08B4CE88(int *self)
{
    for (int i = 0, *p = self; i < 3; i++, p++) {
        if (p[0x14 / 4] != 0) {
            func_0887BA5C(p[0x14 / 4]);
            p[0x14 / 4] = 0;
        }
    }
}
