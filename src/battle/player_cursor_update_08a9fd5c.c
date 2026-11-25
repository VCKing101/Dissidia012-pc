typedef struct Mi { char p[4]; int c4; int c8; int cc; } Mi;
typedef struct Hu { char p[0x12A9]; unsigned char f12a9; } Hu;
typedef struct Pl { char p[4]; Hu *h4; } Pl;
typedef struct S { char p0[0x10]; int n10; char p14[0xA0]; int b4; unsigned bb8; int bbc; unsigned bc0; int bc4; char pc8[0x98]; int h160; } S;
extern int func_08AB327C(int);
extern int func_08AAEC0C(int);
extern int func_08AB249C(int);
extern void func_08A9BDAC(S *, int);
extern Mi *func_08AB2780(int);
extern void func_08A9BEAC(S *, int, int);
extern int func_08AAEF94(int);
extern Pl *func_08AB3264(int);
extern void func_08A97698(Pl *);
extern void func_08A9B8A8(S *);

int func_08A9FD5C(S *self, int a1) {
    int u, v;
    Mi *m;
    Pl *pl;
    if (a1 == 0) {
        return -1;
    }
    u = func_08AB327C(self->h160);
    v = func_08AAEC0C(u);
    if (func_08AB249C(u) == 0) {
        func_08A9BDAC(self, 0);
    } else {
        m = func_08AB2780(u);
        if (m == 0) {
            return -2;
        }
        if (self->n10 < m->c4) {
            func_08A9BEAC(self, 0, v);
            self->n10 = self->n10 + 1;
        } else {
            if (func_08AAEF94(u) == 0) {
                v = m->c8;
            } else {
                v = m->cc;
            }
            func_08A9BEAC(self, 2, v);
            self->n10 = 0;
        }
        pl = func_08AB3264(self->h160);
        if (pl->h4 == 0) {
            return -3;
        }
        if (pl->h4->f12a9 == 1) {
            func_08A97698(func_08AB3264(self->h160));
        }
    }
    func_08A9B8A8(self);
    if (func_08AB249C(u) == 0) {
        if (self->bbc == v) {
            if (self->bc0 >= (unsigned)self->b4) {
                self->bc0 = -1;
                self->bc4 = 0;
            } else {
                v = self->bc4 + 1;
                self->bc4 = v;
                if (v > 1) {
                    self->bb8 = self->bc0;
                }
            }
        } else if (self->bbc != v) {
            self->bbc = v;
            self->bc0 = -1;
            self->bc4 = 0;
        }
    }
    return 0;
}
