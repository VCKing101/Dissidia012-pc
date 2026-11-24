/* Releases the handle at 0x10 (func_088DA060) if set and clears it; loops over 2 slot(s) of 4 bytes. */
extern "C" void func_088DA060(int);
extern "C" void func_089321E4(int *self) {
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0x10 / 4] != 0) {
            func_088DA060(p[0x10 / 4]);
            p[0x10 / 4] = 0;
        }
    }
}
