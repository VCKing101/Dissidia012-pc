/* Waits for the two flags (running the pump), lets the owned object finish (virtuals 4 and 2, then virtual 0 with 2 when flag 0x53 is 1), and clears it. */
struct Owned088EBEBC {
    virtual void v0(int a); virtual void v1(); virtual void v2(void *s); virtual void v3(); virtual void v4(void *s);
};
struct Obj088EBEBC {
    char pad0[0x28]; Owned088EBEBC *owned;
    char pad1[0x50 - 0x2C]; unsigned char f50; unsigned char f51; unsigned char f52; unsigned char f53;
};
extern "C" {
extern int D_089D5EC0;
void func_088EC010(Obj088EBEBC *o);
void func_0885CBA4(void);
void func_08865918(int v);
void func_0886598C(int v);
void func_088EBEBC(Obj088EBEBC *self)
{
    while (self->f50 == 0) {
        func_088EC010(self);
    }
    self->owned->v4(self);
    while (self->f51 == 0) {
        func_088EC010(self);
    }
    func_0885CBA4();
    self->owned->v2(self);
    if (self->f53 == 1) {
        self->owned->v0(2);
    }
    self->owned = 0;
    func_08865918(D_089D5EC0);
    func_0886598C(D_089D5EC0);
    self->f52 = 0;
    self->f53 = 0;
}
}
