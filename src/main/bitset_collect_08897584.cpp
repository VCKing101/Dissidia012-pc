struct Own { char pad[0x78]; char *bits;
  static bool test(Own *o, int n, int i) { return (((unsigned *)(o->bits + n * 32))[i / 32] & (1 << (i % 32))) != 0; } };
extern "C" int func_08892FD0(int, int, unsigned char);
extern "C" int func_08898854(Own *, int);
extern "C" int func_088988B4(Own *);
extern "C" int D_089D6AC8;
extern "C" unsigned func_08897584(Own *self, int a1, unsigned char a2, unsigned char a3);
extern "C" unsigned func_08897584(Own *self, int a1, unsigned char a2, unsigned char a3) {
    if (a3 != 0 && self->bits != 0) {
        unsigned r = 0;
        int n = func_08898854(self, a1);
        if (n > 0) {
            for (int i = 0; i < func_088988B4(self); i++) {
                if (Own::test(self, n, i)) {
                    r |= func_08897584(self, i, a2, 0);
                }
            }
        }
        r |= func_08897584(self, a1, a2, 0);
        return r;
    } else {
        int hit = func_08892FD0(D_089D6AC8, a1, a2);
        if (hit != 0) { return 1; } else { return 0; }
    }
}
