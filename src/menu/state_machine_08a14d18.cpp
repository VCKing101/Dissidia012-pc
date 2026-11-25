extern "C" {
    typedef struct S { char p0[0x400]; char a400[0x480]; char a880[0x40C]; int st; unsigned fl; } S;
    typedef struct H { char p0[0xd80]; unsigned fl; } H;
    typedef struct G { char p0[0x154]; H *h; } G;
    typedef struct Obj { char p0[0xf60]; S *s; } Obj;
    extern G *D_08A23FF0;
    extern int func_08A22948(void *, S *, void *, void *);
    extern void func_08A1EC00(void *, S *, void *, void *);
    extern int func_08A1ED38(void *, S *, void *, void *);
    extern int func_08A12F94(Obj *);
    extern int func_08A22948(void *, S *, void *, void *);
    struct Q { static unsigned char f(unsigned v) { return (v & 4) != 0; } };
    void func_08A14D18(Obj *self) {
        S *s = self->s;
        int r = func_08A22948(D_08A23FF0, s, s->a400, s->a880);
        unsigned fl;
        s = self->s;
        fl = s->fl;
        if ((fl & 1) != 0) {
            if (Q::f(D_08A23FF0->h->fl) != 0) {
                s->fl = fl & 0xfffffffe;
                self->s->st = 0xe;
            }
        } else if ((fl & 0x1000) != 0) {
            r = func_08A1ED38(D_08A23FF0, s, s->a400, s->a880);
            if (r != 0) {
                self->s->fl &= 0xffffefff;
                self->s->st = 1;
            }
        } else if ((fl & 0x10) != 0) {
            r = func_08A1ED38(D_08A23FF0, s, s->a400, s->a880);
            if (r != 0) {
                self->s->fl &= 0xffffffef;
                self->s->st = 5;
            }
        } else if ((fl & 8) != 0) {
            r = func_08A1ED38(D_08A23FF0, s, s->a400, s->a880);
            if (r != 0) {
                self->s->fl &= 0xfffffff7;
                self->s->st = 10;
            }
        } else if (r != 0) {
            if (r == 1) {
                func_08A1EC00(D_08A23FF0, s, s->a400, s->a880);
                self->s->fl |= 0x10;
            } else {
                r = func_08A12F94(self);
                s = self->s;
                if (r != 0) {
                    func_08A1EC00(D_08A23FF0, s, s->a400, s->a880);
                    self->s->fl |= 8;
                } else {
                    func_08A1EC00(D_08A23FF0, s, s->a400, s->a880);
                    self->s->fl |= 0x1000;
                }
            }
        }
    }
}
