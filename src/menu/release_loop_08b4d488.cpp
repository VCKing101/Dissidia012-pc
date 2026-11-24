/* For the 4 handles at self+0x1c, releases each non-zero one with func_0887BA5C and clears it. */
extern "C" void func_0887BA5C(int h);
extern "C" void func_08B4D488(int *self)
{
    for (int i = 0, *p = self; i < 4; i++, p++) {
        if (p[0x1c / 4] != 0) {
            func_0887BA5C(p[0x1c / 4]);
            p[0x1c / 4] = 0;
        }
    }
}
