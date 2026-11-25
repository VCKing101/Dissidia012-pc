typedef struct Item { unsigned char on; char p1[3]; int h4; unsigned char b8; unsigned char b9; char pa[2]; } Item;
typedef struct Pair { int a, b, c; } Pair;
typedef struct F { char p0[4]; int h4; char p8[0xB4]; Item items[2]; } F;
extern "C" {
extern int func_08A22970(int);
extern int func_08A074FC(int);
extern int func_08A15A70(int);
extern unsigned func_089E95E0(int);
extern int func_089E4A70(void *);
extern void func_089E47F4(void *, int, int);
extern void func_089E41CC(void *, unsigned, int);
extern void func_08935828(void *, int, int);
extern int D_08A22AB0;
}

extern "C" int func_089E3CDC(F *self) {
    if (func_08A15A70(func_08A074FC(func_08A22970(D_08A22AB0))) == 0) {
        return 0;
    } else {
        for (int i = 0; i < 2; i++) {
            Item *it = &self->items[i];
            int t;
            unsigned k;
            int h;
            if (it->on == 0) continue;
            t = self->h4 + it->b8 + 1;
            k = func_089E95E0(t % 2);
            if (k >= 2) continue;
            if (func_089E4A70((char *)self + 8) == 1) {
                h = it->h4;
                func_089E47F4((char *)self + 8, h, 0);
                if (it->b9 == 0) {
                    func_089E41CC((char *)self + 0x70, k, it->h4);
                }
            } else {
                h = it->h4;
                if (h != 0) {
                    func_089E47F4((char *)self + 0x3C, h, 1);
                }
            }
        }
        {
            int j = 0;
            int n2 = 0x18;
            int b = (int)self + 0x18;
            do {
                j = 0;
                Pair *d = (Pair *)(b + 0xA4);
                Pair *s = (Pair *)(b + 0x8C);
                do {
                    *d = *s;
                    j++;
                    d++;
                    s++;
                } while (j < 2);
                n2 -= 0x18;
                b -= 0x18;
            } while (n2 > 0);
        }
        func_08935828((char *)self + 0xA4, 0, 0x18);
    }
    return 0;
}
