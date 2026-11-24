/* Releases the handle at 0x4C (func_0887BA5C) if set and clears it; loops over 4 slot(s) of 4 bytes. */
extern "C" void func_0887BA5C(int);
extern "C" void func_0892F10C(int *self) {
    for (int i = 0, *p = self; i < 4; i++, p++) {
        if (p[0x4C / 4] != 0) {
            func_0887BA5C(p[0x4C / 4]);
            p[0x4C / 4] = 0;
        }
    }
}
