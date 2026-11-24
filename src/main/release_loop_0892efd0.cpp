/* Releases the handle at 0x4 (func_088DA060) if set and clears it; loops over 5 slot(s) of 4 bytes. */
extern "C" void func_088DA060(int);
extern "C" void func_0892EFD0(int *self) {
    for (int i = 0, *p = self; i < 5; i++, p++) {
        if (p[0x4 / 4] != 0) {
            func_088DA060(p[0x4 / 4]);
            p[0x4 / 4] = 0;
        }
    }
}
