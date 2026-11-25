extern "C" {
    typedef struct { char p0[0xC]; short k; char p1[0x20 - 0xE]; unsigned short fl; } Nd;
    typedef struct { char p0[0x28]; int mode; char p1[4]; Nd *n30; Nd *n34; Nd *n38; } Obj;
    extern void func_088D9F5C(void *, int, int);
    extern void func_08B4F7F8(Obj *);
    extern void func_08B4F5A0(Obj *);
    extern void func_08B4F658(Obj *);
    extern void func_08B4F8C0(Obj *);
    struct Q { static unsigned char f(Nd *n) { return (n->fl & 0x20) != 0; } };
    void func_08B4FB40(Obj *self) {
        Nd *n;
        switch (self->mode) {
    case 1:
            n = self->n38;
            if (n->k == 2 && Q::f(n) == 1) {
                func_088D9F5C(n, 3, 0);
                func_08B4F7F8(self);
            }
            if (self->n30->k == 1 && Q::f(self->n30) == 1) {
                func_08B4F5A0(self);
            }
            func_08B4F658(self);
            break;
    case 2:
            n = self->n38;
            if (n->k == 2 && Q::f(n) == 1) {
                func_088D9F5C(n, 3, 0);
                func_08B4F8C0(self);
            }
            if (self->n34->k == 0 && Q::f(self->n34) == 1) {
                func_08B4F5A0(self);
            }
            func_08B4F658(self);
            break;
    case 3:
            n = self->n38;
            if (n->k == 2 && Q::f(n) == 1) {
                func_088D9F5C(n, 3, 0);
                func_08B4F8C0(self);
            }
            if (self->n34->k == 0 && Q::f(self->n34) == 1) {
                func_08B4F7F8(self);
            }
            if (self->n30->k == 1 && Q::f(self->n30) == 1) {
                func_08B4F5A0(self);
            }
            func_08B4F658(self);
        }
    }
}
