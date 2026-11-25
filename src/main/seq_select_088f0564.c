typedef struct St9 { char pad0[4]; unsigned char b4; char pad5; unsigned char b6; char pad7[2]; unsigned char b9; } St9;
extern char *D_089D712C;
extern short D_089D7F14;
extern short D_089D7F16;
extern short D_089D7F18;
extern int func_088C4E9C(int a, int b, int c, int *d, int e);

int func_088F0564(short p) {
    St9 *s = (St9 *)(D_089D712C + 0x1DEE0);
    int a = p;
    int r;
    int local;
    if (s->b6 + s->b4 == 0 && (r = func_088C4E9C(1, 0, 0, 0, 0)) != -1 && r != a) {
        D_089D7F18 = 0;
        s->b9 = 1;
        return r;
    }
    r = func_088C4E9C(2, D_089D7F16, D_089D7F14, 0, 0);
    if (r != -1 && r != a) {
        D_089D7F18 = 0;
        D_089D7F16 = D_089D7F14 + 1;
        return r;
    }
    r = func_088C4E9C(3, 0, D_089D7F18, 0, 1);
    if (r != -1 && r != a) {
        D_089D7F18 = 0;
        return r;
    }
    local = 0;
    if (s->b9 == 0) {
        s->b9 = s->b6;
    }
    r = func_088C4E9C(1, s->b9, s->b6 + s->b4, &local, 0);
    if (r != -1 && r != a) {
        D_089D7F18 = 0;
        s->b9 = local + 1;
        return r;
    }
    D_089D7F18 = 0;
    return -1;
}
