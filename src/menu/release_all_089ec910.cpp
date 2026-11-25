extern "C" {
extern void func_088DA060();
extern void func_0887BA5C();
extern void func_08935828(int *, int, int);

void func_089EC910(int *self)
{
    for (int i = 0, *p = self; i < 0xB; i++, p++) {
        if (p[0x2C / 4] != 0) {
            func_088DA060();
            p[0x2C / 4] = 0;
        }
    }
    for (int i = 0, *a = self + 0x60 / 4; i < 3; i++, a += 15) {
        int *b = a;
        int *c = b + 4;
        for (int j = 0, *d = c; j < 1; j++, d++) {
            if (*d != 0) {
                func_088DA060();
            }
        }
        for (int j = 0, *e = c + 1; j < 2; j++, e += 4) {
            for (int k = 0, *f = e; k < 2; k++, f++) {
                if (*f != 0) {
                    func_088DA060();
                }
            }
            for (int k = 0, *f = e; k < 2; k++, f++) {
                if (f[2] != 0) {
                    func_0887BA5C();
                }
            }
        }
        if (b[0] != 0) {
            func_088DA060();
        }
        if (b[1] != 0) {
            func_0887BA5C();
        }
        if (b[2] != 0) {
            func_0887BA5C();
        }
        if (b[3] != 0) {
            func_0887BA5C();
        }
        b[0] = 0;
        b[1] = 0;
        b[2] = 0;
        b[3] = 0;
        func_08935828(c, 0, 4);
        for (int j = 0, *g = a + 5; j < 2; j++, g += 4) {
            func_08935828(g, 0, 8);
            func_08935828(g + 2, 0, 8);
        }
        b[0xD] = 0;
        ((unsigned char *)b)[0x38] = 0;
    }
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0x58 / 4] != 0) {
            func_0887BA5C();
            p[0x58 / 4] = 0;
        }
    }
    self[0x158 / 4] = 0;
}
}
