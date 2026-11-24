/* Releases the handle at 0x44 (func_0887BA5C) if set and clears it; loops over 2 slot(s) of 4 bytes. */
extern "C" void func_0887BA5C(int);
extern "C" void func_08930AA0(int *self) {
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0x44 / 4] != 0) {
            func_0887BA5C(p[0x44 / 4]);
            p[0x44 / 4] = 0;
        }
    }
}
