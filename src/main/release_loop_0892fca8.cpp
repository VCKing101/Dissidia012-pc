/* Releases the handle at 0x34 (func_088DA060) if set and clears it; loops over 1 slot(s) of 4 bytes. */
extern "C" void func_088DA060(int);
extern "C" void func_0892FCA8(int *self) {
    for (int i = 0, *p = self; i < 1; i++, p++) {
        if (p[0x34 / 4] != 0) {
            func_088DA060(p[0x34 / 4]);
            p[0x34 / 4] = 0;
        }
    }
}
