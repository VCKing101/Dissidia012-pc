typedef struct S {
    char p0[8]; int f8; int pc; int f10; char p14[4]; int f18; int f1c; int f20; int f24; char p28[8]; int f30;
    char p34[0x54]; int f88; int f8c; int f90; int f94; char p98[8]; int fa0; int fa4; char pa8[0x20];
    int fc8; int fcc;
} S;
extern int func_08843A80(S *, void *, int, int);
extern void func_0887A644(int, int);
extern void *func_0887B080(int);
extern void func_0887A61C(void *);
extern void func_0887AC24(void *, int);
extern void func_08843448(S *);
extern void func_088434C8(S *);
extern void func_08828AC8(void *);
extern void func_08828A98(void *, int);
extern int func_0885C648(int, int);
extern void func_088105E0(void *);
extern void func_088105FC(void *, int, int);

void func_08843CF4(S *self, int a1, int a2)
{
    int local;
    int r;
    func_08843A80(self, &self->f24, a1, a2);
    if (self->fcc != 0) {
        func_0887A644(self->fcc, 3);
        self->fcc = 0;
    }
    if (func_08843A80(self, &self->fc8, a1, a2) != 0) {
        void *q = 0;
        void *p = func_0887B080(0x10);
        if (p != 0) {
            func_0887A61C(p);
            q = p;
        }
        self->fcc = (int)q;
        func_0887AC24(q, self->fc8);
    }
    func_08843A80(self, &self->f1c, a1, a2);
    local = self->f18;
    if (func_08843A80(self, &local, a1, a2) != 0) {
        self->f18 = local;
    }
    func_08843A80(self, &self->f8c, a1, a2);
    func_08843A80(self, &self->f90, a1, a2);
    func_08843A80(self, &self->f88, a1, a2);
    func_08843A80(self, &self->f30, a1, a2);
    if (func_08843A80(self, &self->f8, a1, a2) != 0) {
        func_08843448(self);
    }
    if (func_08843A80(self, &self->f10, a1, a2) != 0) {
        func_088434C8(self);
    }
    local = self->f20;
    if (func_08843A80(self, &local, a1, a2) != 0) {
        self->f20 = local;
    }
    local = self->f94;
    if (func_08843A80(self, &local, a1, a2) != 0) {
        void *o = &self->f94;
        func_08828AC8(o);
        func_08828A98(o, local);
    }
    local = self->fa4;
    if (local != 0) {
        int a = a2;
        r = func_0885C648(a, local);
        if (func_08843A80(self, &local, a1, a) != 0) {
            void *o = &self->fa0;
            func_088105E0(o);
            func_088105FC(o, local, r);
        }
    }
}
