extern "C" {
    typedef struct W { char p0[0x2a8]; unsigned fl; int t2ac; char p2[0x2cc - 0x2b0]; int st; } W;
    typedef struct Obj { char p0[8]; W *w; } Obj;
    extern int D_08A8B7A8;
    extern int func_08A919B8(Obj *, int);
    extern void func_08A8BC68(Obj *, int);
    extern void func_088D4BF4(int);
    extern void func_08A8E7AC(Obj *);
    extern void func_08A8EB48(Obj *, int);
    extern void func_08A924A8(Obj *, int, int, int);
    extern void func_08A8CCE0(Obj *, int, int);
    extern void func_08A8CBE8(Obj *, int, int);
    extern void func_08A8EF2C(Obj *, int, int);
    extern void func_08A8A440(int, int, int);
    extern void func_08A8EF90(Obj *, int);
    extern void func_08A90E04(Obj *, int);
    extern void func_08A8C46C(Obj *, int);
    extern void func_08A8C3A0(Obj *);
    struct H { static int sel(Obj *self, int mode, int v) {
        int r;
        unsigned fl;
        if (mode == 1) {
            fl = self->w->fl;
            if ((fl & 0x100) != 0 || (fl & 0x20000) != 0) {
                r = -1;
            } else {
                r = func_08A919B8(self, v);
            }
        } else {
            r = func_08A919B8(self, v);
        }
        return r;
    } };
    void func_08A8C9A0(Obj *self, int mode) {
        int v;
        int r;
        unsigned fl;
        v = mode == 0 ? 3 : 2;
        r = H::sel(self, mode, v);
        if (r == 0) {
            if ((self->w->fl & 0x40000) != 0) {
                func_08A8EB48(self, 1);
                self->w->fl |= 0x80000;
                func_088D4BF4(4);
            } else {
                self->w->t2ac = 10000;
                func_08A8BC68(self, 1);
                func_088D4BF4(1);
                func_08A8E7AC(self);
            }
        } else if (r == 1 || r == 2) {
            W *w = self->w;
            fl = w->fl;
            if ((fl & 0x80000) != 0) {
                w->fl = fl & 0xfff7ffff;
                func_08A8EB48(self, 0);
                func_088D4BF4(2);
                goto done;
            } else {
                w->fl = fl & 0xfffbffff;
                func_08A924A8(self, 0, 0, 0);
                func_08A8CCE0(self, 0, 0);
                func_08A8CBE8(self, 0, -1);
                self->w->t2ac = 500;
                func_088D4BF4(2);
                if (mode == 1) {
                    self->w->st = 5;
                }
                func_08A8EF2C(self, 1, 1);
                func_08A8A440(D_08A8B7A8, 1, 0);
                func_08A8EF90(self, 10);
            }
        } else if (r == 3) {
            func_08A8EF90(self, 5);
        }
    done:
        func_08A90E04(self, v);
        func_08A8C46C(self, mode + 3);
        func_08A8C3A0(self);
    }
}
