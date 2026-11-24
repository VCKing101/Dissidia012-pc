/* Releases the handle at 0x18 (func_0887BA5C) if set and clears it; loops over 3 slot(s) of 4 bytes. */
extern "C" void func_0887BA5C(int);
extern "C" void func_0893235C(int *self) {
    for (int i = 0, *p = self; i < 3; i++, p++) {
        if (p[0x18 / 4] != 0) {
            func_0887BA5C(p[0x18 / 4]);
            p[0x18 / 4] = 0;
        }
    }
}
