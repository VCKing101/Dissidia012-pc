/* For the two handles at self+0x38, releases each non-zero one with func_0887BA5C and clears it. */
extern "C" void func_0887BA5C(int h);
extern "C" void func_08B4C6FC(int *self)
{
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0x38 / 4] != 0) {
            func_0887BA5C(p[0x38 / 4]);
            p[0x38 / 4] = 0;
        }
    }
}
