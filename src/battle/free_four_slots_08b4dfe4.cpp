// Calls func_0899BB1C on each of four pointer slots at +0xC and clears them.
extern "C" void func_0899BB1C(void *);
extern "C" void func_08B4DFE4(int *self)
{
    for (int i = 0, *p = self; i < 4; i++, p++) {
        func_0899BB1C((void *)p[3]);
        p[3] = 0;
    }
}
