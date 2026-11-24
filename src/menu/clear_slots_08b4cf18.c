/* Constructor-style init: base ctor, then clears three int arrays and five halfword quads. */
extern void func_08931DA0(void);
void func_08B4CF18(short *self) {
    int i;
    int *p;
    int m;
    func_08931DA0();
    for (i = 0, p = (int *)self; i < 3; i++, p++) { p[0x10 / 4] = 0; }
    for (i = 0, p = (int *)self; i < 4; i++, p++) { p[0x1C / 4] = 0; }
    i = 0;
    m = 0xFFFF;
    for (; i < 5; i++, self += 4) {
        self[0x16] = m;
        self[0x17] = m;
        self[0x18] = m;
        self[0x19] = m;
    }
}
