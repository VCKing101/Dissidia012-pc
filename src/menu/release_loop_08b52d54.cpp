/* For the 2 handles at self+0x3c, releases each non-zero one with func_0887BA5C and clears it. */
extern "C" void func_0887BA5C(int h);
extern "C" void func_08B52D54(int *self)
{
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0x3c / 4] != 0) {
            func_0887BA5C(p[0x3c / 4]);
            p[0x3c / 4] = 0;
        }
    }
}
