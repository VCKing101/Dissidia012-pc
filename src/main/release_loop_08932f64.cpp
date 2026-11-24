/* Releases the handle at 0x2C (func_088DAB44) if set and clears it; loops over 2 slot(s) of 4 bytes. */
extern "C" void func_088DAB44(int);
extern "C" void func_08932F64(int *self) {
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0x2C / 4] != 0) {
            func_088DAB44(p[0x2C / 4]);
            p[0x2C / 4] = 0;
        }
    }
}
