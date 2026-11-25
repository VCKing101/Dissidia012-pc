// Releases each of six pointers (word at +0x34 of consecutive 4-byte steps) with func_0887BA5C and clears them.
extern "C" void func_0887BA5C(int);
extern "C" void func_08B14714(int *self)
{
    for (int i = 0, *p = self; i < 6; i++, p++) {
        if (p[0x34 / 4] != 0) {
            func_0887BA5C(p[0x34 / 4]);
            p[0x34 / 4] = 0;
        }
    }
}
