typedef struct N0 { char p[0xC]; short s0C; } N0;
typedef struct N1 { char p[0x20]; unsigned short s20; } N1;
typedef struct H { char p0[4]; int h4; int h8; char pc[0xC4]; N0 *hd0; N1 *hd4; } H;
extern unsigned func_089E95E0(int);
extern unsigned func_089E98F0(int);
extern int func_089E9C24(int);
extern int func_089E7018(int);
extern int func_089E7038(int, int);
extern void func_088D9F5C(int, int, int);
extern void func_088DA0A8(int, int, void *, void *);
extern void func_088D9B84(int, void *);
extern void func_088D9AF4(int, void *);
extern int D_089E59E0[];
extern int D_089E59F0[];
extern int D_089E5A00[];
extern int D_089E5A10[];

int func_089E57C4(H *self) {
    char bufA[0x40];
    int bufB[1];
    unsigned a, b;
    int r, h, t;
    N0 *n;
    unsigned off;
    a = func_089E95E0(self->h4);
    if (a >= 2) {
        return -1;
    }
    b = func_089E98F0(self->h4);
    if (b >= 2) {
        return -2;
    }
    n = self->hd0;
    off = a * 8 + b * 4;
    if (n->s0C == *(int *)(off + (unsigned)D_089E59E0)) {
        r = func_089E9C24(self->h4);
        if (r == 1) {
            func_088D9F5C((int)self->hd0, *(int *)(off + (unsigned)D_089E59F0), 1);
        }
    } else {
        r = func_089E9C24(self->h4);
        if (r == 0) {
            func_088D9F5C((int)self->hd0, *(int *)(off + (unsigned)D_089E59E0), 1);
        }
    }
    if ((unsigned char)((self->hd4->s20 & 8) != 0) == 0) {
        h = func_089E7018(self->h8);
        if (h == 0) {
            return -2;
        }
        {
            int t2 = func_089E7038(self->h8, D_089E5A00[a]);
            func_088DA0A8(h, t2, bufA, bufB);
            t = (int)self->hd0;
        }
    } else {
        func_088DA0A8((int)self->hd4, D_089E5A10[a], bufA, bufB);
        t = (int)self->hd0;
    }
    func_088D9B84(t, bufA);
    func_088D9AF4((int)self->hd0, bufB);
    return 0;
}
