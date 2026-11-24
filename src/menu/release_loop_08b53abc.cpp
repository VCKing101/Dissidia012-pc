/* For the 1 handles at self+0x38, releases each non-zero one with func_0887BA5C and clears it. */
extern "C" void func_0887BA5C(int h);
extern "C" void func_08B53ABC(int *self)
{
    for (int i = 0, *p = self; i < 1; i++, p++) {
        if (p[0x38 / 4] != 0) {
            func_0887BA5C(p[0x38 / 4]);
            p[0x38 / 4] = 0;
        }
    }
}
