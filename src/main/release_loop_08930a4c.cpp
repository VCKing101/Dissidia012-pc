/* Releases the handle at 0xC (func_088DA060) if set and clears it; loops over 4 slot(s) of 4 bytes. */
extern "C" void func_088DA060(int);
extern "C" void func_08930A4C(int *self) {
    for (int i = 0, *p = self; i < 4; i++, p++) {
        if (p[0xC / 4] != 0) {
            func_088DA060(p[0xC / 4]);
            p[0xC / 4] = 0;
        }
    }
}
