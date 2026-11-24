/* Sets flag bits on the object by mode (0 and 3 set 0x400), the 0x40000000 bit when 0x20000000 is clear, and starts the effect when bit 1 of the second word is set. */
extern int D_089D727C;
extern char D_08AB43D4[];
extern void func_088B1354(int g, int a, void *t, int b, int c);
void func_08AB7030(int self, int mode) {
    unsigned w = *(unsigned *)(self + 0x2C);
    unsigned v = *(unsigned *)(self + 0x30);
    if (mode == 0) {
        w = w | 0x400;
        *(unsigned *)(self + 0x2C) = w;
    }
    if (mode == 3) {
        w = w | 0x400;
        *(unsigned *)(self + 0x2C) = w;
    }
    if ((w & 0x20000000) == 0) {
        *(unsigned *)(self + 0x2C) = w | 0x40000000;
    }
    if ((v & 1) != 0) {
        *(unsigned *)(self + 0x30) = v | 8;
        func_088B1354(D_089D727C, 0, D_08AB43D4, 0, 0xA0);
    }
}
