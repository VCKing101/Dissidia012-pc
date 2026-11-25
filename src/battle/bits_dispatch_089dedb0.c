extern unsigned int D_08AE7CA4;
extern void func_089DE6EC(void *self, int k, void *a, void *b, int dir);
extern void func_089DFCA4(void *a);
extern void func_08AE3BC8(void);

int func_089DEDB0(char *self)
{
    unsigned int bits = D_08AE7CA4;
    char *s;
    if (bits & 0x1) {
        int c = 0;
        char *t = self + 0x78;
        int d = 1;
        s = self + 0x24;
        func_089DE6EC(self, c, s, t, d);
        func_089DFCA4(s);
    }
    if (bits & 0x2) {
        int c = 1;
        char *t = self + 0x24;
        s = self + 0x78;
        func_089DE6EC(self, c, s, t, c);
        func_089DFCA4(s);
    }
    if (bits & 0x4) {
        int c = 2;
        char *u = self + 0xCC;
        char *t = self + 0x24;
        int d = 1;
        func_089DE6EC(self, c, u, t, d);
    }
    if (bits & 0x8) {
        int c = 3;
        char *u = self + 0xCC;
        char *t = self + 0x78;
        int d = 1;
        func_089DE6EC(self, c, u, t, d);
    }
    if (bits & 0x10) {
        int c = 4;
        char *t = self + 0xCC;
        int d = 1;
        s = self + 0x24;
        func_089DE6EC(self, c, s, t, d);
        func_089DFCA4(s);
    }
    if (bits & 0x20) {
        int c = 5;
        char *t = self + 0xCC;
        int d = 1;
        s = self + 0x78;
        func_089DE6EC(self, c, s, t, d);
        func_089DFCA4(s);
    }
    if (bits & 0x40) {
        int c = 6;
        char *t = self + 0x24;
        int d = 1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x80) {
        int c = 7;
        char *t = self + 0x78;
        int d = 1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x100) {
        int c = 8;
        char *t = self + 0xCC;
        int d = 1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x200) {
        int c = 9;
        char *t = self + 0x24;
        int d = -1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x400) {
        int c = 10;
        char *t = self + 0x78;
        int d = -1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x800) {
        int c = 11;
        char *t = self + 0xCC;
        int d = -1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x1000) {
        int c = 12;
        char *t = self + 0x24;
        int d = 1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x2000) {
        int c = 13;
        char *t = self + 0x78;
        int d = 1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x4000) {
        int c = 14;
        char *t = self + 0xCC;
        int d = 1;
        func_089DE6EC(self, c, 0, t, d);
    }
    if (bits & 0x8000) {
        int c = 15;
        char *t = self + 0x24;
        int d = -1;
        func_089DE6EC(self, c, 0, t, d);
        func_089DFCA4(t);
    }
    if (bits & 0x10000) {
        int c = 16;
        char *t = self + 0x78;
        int d = -1;
        func_089DE6EC(self, c, 0, t, d);
        func_089DFCA4(t);
    }
    func_08AE3BC8();
    return 0;
}
